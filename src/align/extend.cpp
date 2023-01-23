/****
DIAMOND protein aligner
Copyright (C) 2020 Max Planck Society for the Advancement of Science e.V.

Code developed by Benjamin Buchfink <benjamin.buchfink@tue.mpg.de>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
****/

#include <algorithm>
#include <utility>
#include <math.h>
#include <mutex>
#include "extend.h"
#include "../data/queries.h"
#include "../basic/config.h"
#include "../stats/hauser_correction.h"
#include "target.h"
#include "../dp/dp.h"
#include "../util/log_stream.h"
#include "../data/reference.h"
#include "../util/system.h"
#include "../util/util.h"
#include "global_ranking/global_ranking.h"
#include "../masking/masking.h"
#include "../search/hit.h"
#include "load_hits.h"

using std::vector;
using std::list;
using std::array;
using std::pair;
using std::endl;
using std::move;

const SEMap<Extension::Mode> EnumTraits<Extension::Mode>::from_string = {
	{ "banded-fast", Extension::Mode::BANDED_FAST},
	{ "banded-slow", Extension::Mode::BANDED_SLOW},
	{ "full", Extension::Mode::FULL},
	{ "global", Extension::Mode::GLOBAL}
};

namespace Extension {

const std::map<Sensitivity, Mode> default_ext_mode = {
	{ Sensitivity::FAST, Mode::BANDED_FAST},
	{ Sensitivity::DEFAULT, Mode::BANDED_FAST},
	{ Sensitivity::MID_SENSITIVE, Mode::BANDED_FAST},
	{ Sensitivity::SENSITIVE, Mode::BANDED_FAST},
	{ Sensitivity::MORE_SENSITIVE, Mode::BANDED_SLOW},
	{ Sensitivity::VERY_SENSITIVE, Mode::BANDED_SLOW},
	{ Sensitivity::ULTRA_SENSITIVE, Mode::BANDED_SLOW}
};

constexpr size_t MAX_CHUNK_SIZE = 400, MIN_CHUNK_SIZE = 128;

size_t ranking_chunk_size(size_t target_count, size_t ref_letters) {
	if (config.no_ranking || config.global_ranking_targets > 0)
		return target_count;
	if (config.ext_chunk_size > 0)
		return config.ext_chunk_size;
	const double default_letters = config.sensitivity >= Sensitivity::VERY_SENSITIVE ? 800 * 1e6 : 2 * 1e9;
	const size_t block_mult = std::max(size_t(std::round((double)ref_letters / default_letters)), (size_t)1);
	if (config.toppercent < 100.0)
		return MIN_CHUNK_SIZE * block_mult;
	return std::max(MIN_CHUNK_SIZE, std::min(make_multiple(config.max_alignments, (size_t)32), MAX_CHUNK_SIZE)) * block_mult;
}

size_t chunk_size_multiplier(const FlatArray<SeedHit>& seed_hits, int query_len) {
	return seed_hits.size() * query_len / seed_hits.data_size() < config.seedhit_density ? config.chunk_size_multiplier : 1;
}

static size_t lazy_masking(const vector<uint32_t>& target_block_ids, Block& targets, const MaskingAlgo algo) {
	if (algo == MaskingAlgo::NONE)
		return 0;
	vector<Letter> seq;
	const Masking& masking = Masking::get();
	size_t n = 0;
	for (uint32_t t : target_block_ids)
		if (targets.fetch_seq_if_unmasked(t, seq)) {
			masking(seq.data(), seq.size(), algo, t);
			targets.write_masked_seq(t, seq);
			++n;
		}
	return n;
}

static HspValues first_round_hspv() {
	HspValues first_round = HspValues::NONE;
	if (config.min_id > 0)
		first_round |= HspValues::IDENT | HspValues::LENGTH;
	if (config.query_cover > 0)
		first_round |= HspValues::QUERY_COORDS;
	if (config.subject_cover > 0)
		first_round |= HspValues::TARGET_COORDS;
	return first_round;
}

vector<Target> extend(size_t query_id,
	const Sequence *query_seq,
	int source_query_len,
	const Bias_correction *query_cb,
	const Stats::Composition& query_comp,
	FlatArray<SeedHit> &seed_hits,
	vector<uint32_t> &target_block_ids,
	const Search::Config& cfg,
	Statistics& stat,
	DP::Flags flags,
	const HspValues hsp_values)
{
	static const Loc GAPPED_FILTER_MIN_QLEN = 85;
	stat.inc(Statistics::TARGET_HITS2, target_block_ids.size());
	task_timer timer(flag_any(flags, DP::Flags::PARALLEL) ? config.target_parallel_verbosity : UINT_MAX);

	if (cfg.lazy_masking && !config.global_ranking_targets)
		stat.inc(Statistics::MASKED_LAZY, lazy_masking(target_block_ids, *cfg.target, cfg.target_masking));

	if (cfg.gapped_filter_evalue > 0.0 && config.global_ranking_targets == 0 && (!align_mode.query_translated || query_seq[0].length() >= GAPPED_FILTER_MIN_QLEN)) {
		timer.go("Computing gapped filter");
		gapped_filter(query_seq, query_cb, seed_hits, target_block_ids, stat, flags, cfg);
		if (!flag_any(flags, DP::Flags::PARALLEL))
			stat.inc(Statistics::TIME_GAPPED_FILTER, timer.microseconds());
	}
	stat.inc(Statistics::TARGET_HITS3, target_block_ids.size());

	timer.go("Computing chaining");
	vector<WorkTarget> targets = ungapped_stage(query_seq, query_cb, query_comp, seed_hits, target_block_ids, flags, stat, *cfg.target, cfg.extension_mode);
	if (!flag_any(flags, DP::Flags::PARALLEL))
		stat.inc(Statistics::TIME_CHAINING, timer.microseconds());

	return align(targets, query_seq, query_cb, source_query_len, flags, hsp_values, cfg.extension_mode, stat);
}

vector<Match> extend(
	size_t query_id,
	const Search::Config& cfg,
	Statistics& stat,
	DP::Flags flags,
	FlatArray<SeedHit>& seed_hits,
	vector<uint32_t>& target_block_ids,
	const vector<TargetScore>& target_scores)
{
	const unsigned UNIFIED_TARGET_LEN = 50;
	const unsigned contexts = align_mode.query_contexts;
	vector<Sequence> query_seq;
	vector<Bias_correction> query_cb;
	const char* query_title = cfg.query->ids()[query_id];

	if (config.log_query || (flag_any(flags, DP::Flags::PARALLEL) && !config.swipe_all))
		log_stream << "Query=" << query_title << " Hits=" << seed_hits.data_size() << endl;

	for (unsigned i = 0; i < contexts; ++i)
		query_seq.push_back(cfg.query->seqs()[query_id * contexts + i]);
	const unsigned query_len = (unsigned)query_seq.front().length();

	task_timer timer(flag_any(flags, DP::Flags::PARALLEL) ? config.target_parallel_verbosity : UINT_MAX);
	if (Stats::CBS::hauser(config.comp_based_stats)) {
		timer.go("Computing CBS");
		for (unsigned i = 0; i < contexts; ++i)
			query_cb.emplace_back(query_seq[i]);
		timer.finish();
	}
	Stats::Composition query_comp;
	if (Stats::CBS::matrix_adjust(config.comp_based_stats))
		query_comp = Stats::composition(query_seq[0]);

	const int source_query_len = align_mode.query_translated ? (int)cfg.query->source_seqs()[query_id].length() : (int)cfg.query->seqs()[query_id].length();
	const size_t target_count = target_block_ids.size();
	const size_t chunk_size = ranking_chunk_size(target_count, cfg.target->seqs().letters());
	vector<TargetScore>::const_iterator i0 = target_scores.cbegin(), i1 = i0 + std::min((ptrdiff_t)chunk_size, target_scores.cend() - i0);

	if (config.toppercent == 100.0 && config.min_bit_score == 0.0)
#ifdef EVAL_TARGET
		while (i1 < target_scores.cend() && i1->evalue <= config.max_evalue && size_t(i1 - i0) < config.max_alignments) ++i1;
#else
		while (i1 < target_scores.cend() && score_matrix.evalue(i1->score, query_len, UNIFIED_TARGET_LEN) <= config.max_evalue) i1 += 16;
#endif

	const int low_score = config.query_memory ? memory->low_score(query_id) : 0;
	const size_t previous_count = config.query_memory ? memory->count(query_id) : 0;


	const HspValues first_round = first_round_hspv();

	//size_t multiplier = 1;
	int tail_score = 0;
	thread_local FlatArray<SeedHit> seed_hits_chunk;
	thread_local vector<uint32_t> target_block_ids_chunk;

	vector<Target> aligned_targets;
	while (i0 < target_scores.cend()) {
		seed_hits_chunk.clear();
		target_block_ids_chunk.clear();
		const size_t current_chunk_size = (size_t)(i1 - i0);
		const bool multi_chunk = current_chunk_size < target_scores.size();
		if (config.query_memory && memory->ranking_failed_count(query_id) >= chunk_size && memory->ranking_low_score(query_id) >= i0->score)
			break;

		if (multi_chunk)
			for (vector<TargetScore>::const_iterator j = i0; j < i1; ++j) {
				target_block_ids_chunk.push_back(target_block_ids[j->target]);
				seed_hits_chunk.push_back(seed_hits.begin(j->target), seed_hits.end(j->target));
			}

		//multiplier = std::max(multiplier, chunk_size_multiplier(seed_hits_chunk, (int)query_seq.front().length()));

		vector<Target> v = extend(query_id, query_seq.data(), source_query_len, query_cb.data(), query_comp, multi_chunk ? seed_hits_chunk : seed_hits, multi_chunk ? target_block_ids_chunk : target_block_ids, cfg, stat, flags, first_round);
		const size_t n = v.size();
		stat.inc(Statistics::TARGET_HITS4, v.size());
		bool new_hits = false;
		if (multi_chunk)
			new_hits = append_hits(aligned_targets, v.begin(), v.end(), chunk_size, source_query_len, query_title, query_seq.front(), *cfg.target);
		else
			aligned_targets = move(v);

		if (n == 0 || !new_hits) {
			if (config.query_memory && current_chunk_size >= chunk_size)
				memory->update_failed_count(query_id, current_chunk_size, (i1 - 1)->score);
			const double tail_bit_score = score_matrix.bitscore((i1 - 1)->score);
			if (tail_score == 0 || double((i1 - 1)->score) / (double)tail_score <= config.ranking_score_drop_factor || tail_bit_score < config.ranking_cutoff_bitscore)
				break;
		}
		else
			tail_score = (i1 - 1)->score;

		i0 = i1;
		i1 = std::min(i1 + std::min(chunk_size, MAX_CHUNK_SIZE), target_scores.cend());
	}

	if (config.swipe_all)
		aligned_targets = full_db_align(query_seq.data(), query_cb.data(), flags, first_round, stat, *cfg.target);

	/*if (multiplier > 1)
		stat.inc(Statistics::HARD_QUERIES);*/

	timer.go("Computing culling");
	culling(aligned_targets, source_query_len, query_title, query_seq.front(), 0, *cfg.target);
	if (config.query_memory)
		memory->update(query_id, aligned_targets.begin(), aligned_targets.end());
	stat.inc(Statistics::TARGET_HITS5, aligned_targets.size());
	timer.finish();

	vector<Match> matches = align(aligned_targets, query_seq.data(), query_cb.data(), source_query_len, flags, first_round, cfg.extension_mode, stat);
	std::sort(matches.begin(), matches.end(), config.toppercent == 100.0 ? Match::cmp_evalue : Match::cmp_score);
	return matches;
}

vector<Match> extend(size_t query_id, Search::Hit* begin, Search::Hit* end, const Search::Config &cfg, Statistics &stat, DP::Flags flags) {
	task_timer timer(flag_any(flags, DP::Flags::PARALLEL) ? config.target_parallel_verbosity : UINT_MAX);
	timer.go("Loading seed hits");
	thread_local FlatArray<SeedHit> seed_hits;
	thread_local vector<uint32_t> target_block_ids;
	thread_local vector<TargetScore> target_scores;
	load_hits(begin, end, seed_hits, target_block_ids, target_scores, cfg.target->seqs());
	stat.inc(Statistics::TARGET_HITS0, target_block_ids.size());
	stat.inc(Statistics::TIME_LOAD_HIT_TARGETS, timer.microseconds());
	timer.finish();
	const size_t target_count = target_block_ids.size();	
	const size_t chunk_size = ranking_chunk_size(target_count, cfg.target->seqs().letters());

	if (chunk_size < target_count || config.global_ranking_targets > 0) {
		timer.go("Sorting targets by score");
		std::sort(target_scores.begin(), target_scores.end());
		stat.inc(Statistics::TIME_SORT_TARGETS_BY_SCORE, timer.microseconds());
		timer.finish();
		if (config.global_ranking_targets > 0)
			return GlobalRanking::ranking_list(query_id, target_scores.begin(), target_scores.end(), target_block_ids.begin(), seed_hits, cfg);
	}

	return extend(query_id, cfg, stat, flags, seed_hits, target_block_ids, target_scores);
}

}