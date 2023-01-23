/****
DIAMOND protein aligner
Copyright (C) 2013-2020 Max Planck Society for the Advancement of Science e.V.
                        Benjamin Buchfink
                        Eberhard Karls Universitaet Tuebingen

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

#include <memory>
#include "../basic/value.h"
#include "align.h"
#include "../data/reference.h"
#include "../output/output_format.h"
#include "../util/queue.h"
#include "../output/output.h"
#include "legacy/query_mapper.h"
#include "../util/async_buffer.h"
#if _MSC_FULL_VER == 191627042
#include "../util/algo/merge_sort.h"
#endif
#include "extend.h"
#include "../util/algo/radix_sort.h"
#include "target.h"
#define _REENTRANT
#include "../lib/ips4o/ips4o.hpp"

using std::get;
using std::tuple;
using std::unique_ptr;

DpStat dp_stat;

struct Align_fetcher
{
	static void init(size_t qbegin, size_t qend, Search::Hit* begin, Search::Hit* end)
	{
		it_ = begin;
		end_ = end;
		queue_ = unique_ptr<Queue>(new Queue(qbegin, qend));
	}
	bool operator()(size_t query)
	{
		const unsigned q = (unsigned)query,
			c = align_mode.query_contexts;
		begin = it_;
		while (it_ < end_ && it_->query_ / c == q)
			++it_;
		end = it_;
		this->query = query;
		target_parallel = (end - begin > config.query_parallel_limit) && (config.frame_shift == 0 || (config.toppercent < 100 && config.query_range_culling));
		return target_parallel;
	}
	bool get()
	{
		return queue_->get(*this) != Queue::end;
	}
	void release() {
		if (target_parallel)
			queue_->release();
	}
	size_t query;
	Search::Hit* begin, *end;
	bool target_parallel;
private:	
	static Search::Hit* it_, *end_;
	static unique_ptr<Queue> queue_;
};

unique_ptr<Queue> Align_fetcher::queue_;
Search::Hit* Align_fetcher::it_;
Search::Hit* Align_fetcher::end_;

TextBuffer* legacy_pipeline(Align_fetcher &hits, Search::Config& cfg, Statistics &stat) {
	if (hits.end == hits.begin) {
		TextBuffer *buf = nullptr;
		if (!blocked_processing && *output_format != Output_format::daa && config.report_unaligned != 0) {
			buf = new TextBuffer;
			const char *query_title = cfg.query->ids()[hits.query];
			output_format->print_query_intro(hits.query, query_title, cfg.query->source_len((unsigned)hits.query), *buf, true, cfg);
			output_format->print_query_epilog(*buf, query_title, true, cfg);
		}
		return buf;
	}

	QueryMapper *mapper = new ExtensionPipeline::BandedSwipe::Pipeline(hits.query, hits.begin, hits.end, dp_stat, cfg, hits.target_parallel);

	task_timer timer("Initializing mapper", hits.target_parallel ? 3 : UINT_MAX);
	mapper->init();
	timer.finish();
	mapper->run(stat);

	timer.go("Generating output");
	TextBuffer *buf = nullptr;
	if (*output_format != Output_format::null) {
		buf = new TextBuffer;
		const bool aligned = mapper->generate_output(*buf, stat);
		if (aligned && cfg.track_aligned_queries) {
			query_aligned_mtx.lock();
			if (!query_aligned[hits.query]) {
				query_aligned[hits.query] = true;
				++cfg.iteration_query_aligned;
			}
			query_aligned_mtx.unlock();
		}
	}
	delete mapper;
	return buf;
}

void align_worker(size_t thread_id, Search::Config* cfg)
{
	try {
		Align_fetcher hits;
		Statistics stat;
		DpStat dp_stat;
		const bool parallel = config.swipe_all && (cfg->target->seqs().size() >= cfg->query->seqs().size());
		while (hits.get()) {
			if (config.frame_shift != 0) {
				TextBuffer* buf = legacy_pipeline(hits, *cfg, stat);
				OutputSink::get().push(hits.query, buf);
				hits.release();
				continue;
			}
			task_timer timer;
			vector<Extension::Match> matches = Extension::extend(hits.query, hits.begin, hits.end, *cfg, stat, hits.target_parallel || parallel ? DP::Flags::PARALLEL : DP::Flags::NONE);
			TextBuffer* buf = blocked_processing ? Extension::generate_intermediate_output(matches, hits.query, *cfg) : Extension::generate_output(matches, hits.query, stat, *cfg);
			if (!matches.empty() && cfg->track_aligned_queries) {
				std::lock_guard<std::mutex> lock(query_aligned_mtx);
				if (!query_aligned[hits.query]) {
					query_aligned[hits.query] = true;
					++cfg->iteration_query_aligned;
				}
			}
			OutputSink::get().push(hits.query, buf);
			if (hits.target_parallel)
				stat.inc(Statistics::TIME_TARGET_PARALLEL, timer.microseconds());
			hits.release();
			if (config.swipe_all && !config.no_heartbeat && (hits.query % 100 == 0))
				log_stream << "Queries = " << hits.query << std::endl;
		}
		statistics += stat;
		::dp_stat += dp_stat;
	}
	catch (std::exception& e) {
		exit_with_error(e);
	}
}

void align_queries(Consumer* output_file, Search::Config& cfg)
{
		
	size_t max_size = std::min(size_t(config.chunk_size*1e9 * 10 * 2) / cfg.index_chunks / 3, config.trace_pt_fetch_size);
	if (config.memory_limit != 0.0)
		max_size = std::max(max_size, size_t(config.memory_limit * 1e9));
	pair<size_t, size_t> query_range;
	if (Stats::CBS::avg_matrix(config.comp_based_stats)) {
		Extension::target_matrices.insert(Extension::target_matrices.end(), cfg.target->seqs().size(), nullptr);
		Extension::target_matrix_count = 0;
	}

	task_timer timer(nullptr, 3);

	if (!blocked_processing && !cfg.iterated())
		cfg.db->init_random_access(current_query_chunk, 0, false);
	
	cfg.seed_hit_buf->load(max_size);

	while (true) {
		timer.go("Loading trace points");				
		tuple<vector<Search::Hit>*, size_t, size_t> input = cfg.seed_hit_buf->retrieve();
		if (get<0>(input) == nullptr)
			break;
		statistics.inc(Statistics::TIME_LOAD_SEED_HITS, timer.microseconds());
		vector<Search::Hit>* hit_buf = get<0>(input);
		query_range = { get<1>(input), get<2>(input) };
		cfg.seed_hit_buf->load(max_size);

		timer.go("Sorting trace points");
#if _MSC_FULL_VER == 191627045
		radix_sort<Search::Hit, Search::Hit::Query>(hit_buf->data(), hit_buf->data() + hit_buf->size(), (uint32_t)query_range.second * align_mode.query_contexts, config.threads_);
#else
		ips4o::parallel::sort(hit_buf->data(), hit_buf->data() + hit_buf->size(), std::less<Search::Hit>(), config.threads_);
#endif
		statistics.inc(Statistics::TIME_SORT_SEED_HITS, timer.microseconds());

		timer.go("Computing alignments");
		Align_fetcher::init(query_range.first, query_range.second, hit_buf->data(), hit_buf->data() + hit_buf->size());
		OutputSink::instance = unique_ptr<OutputSink>(new OutputSink(query_range.first, output_file));
		vector<std::thread> threads;
		if (config.verbosity >= 3 && config.load_balancing == Config::query_parallel && !config.no_heartbeat && !config.swipe_all)
			threads.emplace_back(heartbeat_worker, query_range.second, &cfg);
		size_t n_threads = config.threads_align == 0 ? config.threads_ : config.threads_align;
		if (config.load_balancing == Config::target_parallel || (config.swipe_all && (cfg.target->seqs().size() >= cfg.query->seqs().size())))
			n_threads = 1;
		for (size_t i = 0; i < n_threads; ++i)
			threads.emplace_back(align_worker, i, &cfg);
		for (auto &t : threads)
			t.join();
		statistics.inc(Statistics::TIME_EXT, timer.microseconds());
		
		timer.go("Deallocating buffers");
		delete hit_buf;
	}
	statistics.max(Statistics::SEARCH_TEMP_SPACE, cfg.seed_hit_buf->total_disk_size());
	for (auto i : Extension::target_matrices)
		delete[] i;
	Extension::target_matrices.clear();
	statistics.inc(Statistics::MATRIX_ADJUST_COUNT, Extension::target_matrix_count);

	if (!blocked_processing && !cfg.iterated())
		cfg.db->end_random_access(false);
}