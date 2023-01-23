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

#include "target.h"
#include "../search/hit.h"
#include "../data/sequence_set.h"

namespace Extension {

template<typename It>
void load_hits(It begin, It end, FlatArray<SeedHit> &hits, vector<uint32_t> &target_block_ids, vector<TargetScore> &target_scores, const SequenceSet& ref_seqs) {
	hits.clear();
	hits.reserve(end - begin);
	target_block_ids.clear();
	target_scores.clear();
	if (begin >= end)
		return;
	std::sort(begin, end, Search::Hit::CmpSubject());
	const size_t total_subjects = ref_seqs.size();
	unsigned target_len;
	uint32_t target = UINT32_MAX;
	uint16_t score = 0;
#ifdef HIT_KEEP_TARGET_ID
	if(true) {
#else
	if (std::log2(total_subjects) * (end - begin) < total_subjects / 10) {
#endif
		for (auto i = begin; i < end; ++i) {
#ifdef HIT_KEEP_TARGET_ID
			std::pair<size_t, size_t> l{ i->target_block_id, (size_t)i->subject_ - ref_seqs.position(i->target_block_id, 0) };
#else
			std::pair<size_t, size_t> l = ref_seqs.local_position((uint64_t)i->subject_);
#endif
			const uint32_t t = (uint32_t)l.first;
			if (t != target) {
				if (target != UINT32_MAX) {
#ifdef EVAL_TARGET
					target_scores.push_back({ uint32_t(target_block_ids.size() - 1), score, score_matrix.evalue(score, query_len, target_len) });
#else
					target_scores.push_back({ uint32_t(target_block_ids.size() - 1), score });
#endif
					score = 0;
				}
				hits.next();
				target = t;
				target_len = (unsigned)ref_seqs[target].length();
				target_block_ids.push_back(target);
			}
			hits.push_back({ (int)i->seed_offset_, (int)l.second, i->score_, i->query_ % align_mode.query_contexts });
			score = std::max(score, i->score_);
		}
	}
	else {
		typename vector<size_t>::const_iterator limit_begin = ref_seqs.limits_begin(), it = limit_begin;
		for (auto i = begin; i < end; ++i) {
			const size_t subject_offset = (uint64_t)i->subject_;
			while (*it <= subject_offset) ++it;
			uint32_t t = (uint32_t)(it - limit_begin) - 1;
			if (t != target) {
				if (target != UINT32_MAX) {
#ifdef EVAL_TARGET
					target_scores.push_back({ uint32_t(target_block_ids.size() - 1), score, score_matrix.evalue(score, query_len, target_len) });
#else
					target_scores.push_back({ uint32_t(target_block_ids.size() - 1), score });
#endif
					score = 0;
				}
				hits.next();
				target_block_ids.push_back(t);
				target = t;
				target_len = (unsigned)ref_seqs[target].length();
			}
			hits.push_back({ (int)i->seed_offset_, (int)(subject_offset - *(it - 1)), i->score_, i->query_ % align_mode.query_contexts });
			score = std::max(score, i->score_);
		}
	}
	if (target != UINT32_MAX)
#ifdef EVAL_TARGET
		target_scores.push_back({ uint32_t(target_block_ids.size() - 1), score, score_matrix.evalue(score, query_len, target_len) });
#else
		target_scores.push_back({ uint32_t(target_block_ids.size() - 1), score });
#endif
}

}