/****
DIAMOND protein aligner
Copyright (C) 2021 Max Planck Society for the Advancement of Science e.V.

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

#include <array>
#include <algorithm>
#include <limits.h>
#include <utility>
#include <atomic>
#include <mutex>
#include <limits.h>
#include "../basic/config.h"
#include "../stats/hauser_correction.h"
#include "target.h"
#include "../dp/ungapped.h"
#include "target.h"
#include "../data/reference.h"
#include "../util/log_stream.h"
#include "../util/parallel/thread_pool.h"
#include "../chaining/chaining.h"
#include "../dp/dp.h"

using std::array;
using std::vector;
using std::list;
using std::atomic;
using std::mutex;

namespace Extension {

std::vector<int16_t*> target_matrices;
std::mutex target_matrices_lock;
atomic<size_t> target_matrix_count(0);

WorkTarget::WorkTarget(size_t block_id, const Sequence& seq, int query_len, const Stats::Composition& query_comp, const int16_t** query_matrix) :
	block_id(block_id),
	seq(seq)
{
	ungapped_score.fill(0);
	if (config.comp_based_stats == Stats::CBS::HAUSER_AND_AVG_MATRIX_ADJUST) {
		const int l = (int)seq.length();
		const auto c = Stats::composition(seq);
		auto r = Stats::s_TestToApplyREAdjustmentConditional(query_len, l, query_comp.data(), c.data(), score_matrix.background_freqs());
		if (r == Stats::eCompoScaleOldMatrix)
			return;
		if (*query_matrix == nullptr) {
			*query_matrix = Stats::make_16bit_matrix(Stats::CompositionMatrixAdjust(query_len, query_len, query_comp.data(), query_comp.data(), Stats::CBS::AVG_MATRIX_SCALE, score_matrix.ideal_lambda(), score_matrix.joint_probs(), score_matrix.background_freqs()));
			++target_matrix_count;
		}
		if (target_matrices[block_id] == nullptr) {
			int16_t* target_matrix = Stats::make_16bit_matrix(Stats::CompositionMatrixAdjust(l, l, c.data(), c.data(), Stats::CBS::AVG_MATRIX_SCALE, score_matrix.ideal_lambda(), score_matrix.joint_probs(), score_matrix.background_freqs()));
			bool del = false;
			{
				std::lock_guard<std::mutex> lock(target_matrices_lock);
				if (target_matrices[block_id] == nullptr)
					target_matrices[block_id] = target_matrix;
				else del = true;
			}
			if (del)
				delete[] target_matrix;
			++target_matrix_count;
		}
		matrix = Stats::TargetMatrix(*query_matrix, target_matrices[block_id]);
	}
	else
		matrix = Stats::TargetMatrix(query_comp, query_len, seq);
}

WorkTarget ungapped_stage(FlatArray<SeedHit>::Iterator begin, FlatArray<SeedHit>::Iterator end, const Sequence *query_seq, const Bias_correction *query_cb, const Stats::Composition& query_comp, const int16_t** query_matrix, uint32_t block_id, Statistics& stat, const Block& targets, const Mode mode) {
	array<vector<Diagonal_segment>, MAX_CONTEXT> diagonal_segments;
	task_timer timer;
	const SequenceSet& ref_seqs = targets.seqs(), &ref_seqs_unmasked = targets.unmasked_seqs();
	const bool masking = config.comp_based_stats == Stats::CBS::COMP_BASED_STATS_AND_MATRIX_ADJUST ? Stats::use_seg_masking(query_seq[0], ref_seqs_unmasked[block_id]) : true;
	WorkTarget target(block_id, masking ? ref_seqs[block_id] : ref_seqs_unmasked[block_id], Stats::count_true_aa(query_seq[0]), query_comp, query_matrix);
	stat.inc(Statistics::TIME_MATRIX_ADJUST, timer.microseconds());
	if (!Stats::CBS::avg_matrix(config.comp_based_stats) && target.adjusted_matrix())
		stat.inc(Statistics::MATRIX_ADJUST_COUNT);

	if (mode == Mode::FULL) {
		for (FlatArray<SeedHit>::Iterator hit = begin; hit < end; ++hit)
			target.ungapped_score[hit->frame] = std::max(target.ungapped_score[hit->frame], hit->score);
		return target;
	}
	if (end - begin == 1 && align_mode.query_translated) {
		target.ungapped_score[begin->frame] = begin->score;
		target.hsp[begin->frame].emplace_back(begin->diag(), begin->diag(), begin->score, begin->frame, interval(), interval());
		return target;
	}
	std::sort(begin, end);
	for (FlatArray<SeedHit>::Iterator hit = begin; hit < end; ++hit) {
		target.ungapped_score[hit->frame] = std::max(target.ungapped_score[hit->frame], hit->score);
		if (!diagonal_segments[hit->frame].empty() && diagonal_segments[hit->frame].back().diag() == hit->diag() && diagonal_segments[hit->frame].back().subject_end() >= hit->j)
			continue;
		const Diagonal_segment d = xdrop_ungapped(query_seq[hit->frame], target.seq, hit->i, hit->j);
		if (d.score > 0) {
			diagonal_segments[hit->frame].push_back(d);
		}
	}
	for (int frame = 0; frame < align_mode.query_contexts; ++frame) {
		if (diagonal_segments[frame].empty())
			continue;
		std::stable_sort(diagonal_segments[frame].begin(), diagonal_segments[frame].end(), Diagonal_segment::cmp_diag);
		pair<int, list<Hsp_traits>> hsp = greedy_align(query_seq[frame], target.seq, diagonal_segments[frame].begin(), diagonal_segments[frame].end(), config.log_extend, frame);
		target.hsp[frame] = std::move(hsp.second);
		target.hsp[frame].sort(Hsp_traits::cmp_diag);
	}
	return target;
}

void ungapped_stage_worker(size_t i, size_t thread_id, const Sequence *query_seq, const Bias_correction *query_cb, const Stats::Composition* query_comp, FlatArray<SeedHit> *seed_hits, const uint32_t* target_block_ids, vector<WorkTarget> *out, mutex *mtx, Statistics* stat, const Block* targets, const Mode mode) {
	Statistics stats;
	const int16_t* query_matrix = nullptr;
	WorkTarget target = ungapped_stage(seed_hits->begin(i), seed_hits->end(i), query_seq, query_cb, *query_comp, &query_matrix, target_block_ids[i], stats, *targets, mode);
	{
		std::lock_guard<mutex> guard(*mtx);
		out->push_back(std::move(target));
		*stat += stats;
	}
	delete[] query_matrix;
}

vector<WorkTarget> ungapped_stage(const Sequence *query_seq, const Bias_correction *query_cb, const Stats::Composition& query_comp, FlatArray<SeedHit> &seed_hits, const vector<uint32_t>& target_block_ids, DP::Flags flags, Statistics& stat, const Block& target_block, const Mode mode) {
	vector<WorkTarget> targets;
	if (target_block_ids.size() == 0)
		return targets;
	targets.reserve(target_block_ids.size());
	const int16_t* query_matrix = nullptr;
	if (flag_any(flags, DP::Flags::PARALLEL)) {
		mutex mtx;
		Util::Parallel::scheduled_thread_pool_auto(config.threads_, seed_hits.size(), ungapped_stage_worker, query_seq, query_cb, &query_comp, &seed_hits, target_block_ids.data(), &targets, &mtx, &stat, &target_block, mode);
	}
	else {
		for (size_t i = 0; i < target_block_ids.size(); ++i)
			targets.push_back(ungapped_stage(seed_hits.begin(i), seed_hits.end(i), query_seq, query_cb, query_comp, &query_matrix, target_block_ids[i], stat, target_block, mode));
	}

	delete[] query_matrix;
	return targets;
}

}