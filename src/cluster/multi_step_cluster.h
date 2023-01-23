/****
DIAMOND protein aligner
Copyright (C) 2013-2018 Benjamin Buchfink <buchfink@gmail.com>

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

#pragma once
#include <algorithm>
#include <stdexcept>
#include <stdio.h>
#include <memory>
#include <fstream>
#include <limits>
#include "../util/system/system.h"
#include "../basic/config.h"
#include "../data/reference.h"
#include "../run/workflow.h"
#include "../util/io/consumer.h"
#include "../util/algo/algo.h"
#include "../basic/statistics.h"
#include "../util/log_stream.h"
#include "../dp/dp.h"
#include "../masking/masking.h"
#include "cluster.h"
#include <unordered_map>
#include <numeric>
#include "../util/io/temp_file.h"
#include "disjoint_set.h"

namespace Workflow { namespace Cluster{ 

	struct NodEdgSet {
		uint32_t nodes;
		size_t edges;
		uint32_t set;
	};

class MultiStep : public ClusteringAlgorithm {
private:
	BitVector rep_bitset(const vector<int> &centroid, const BitVector *superset = nullptr);
	vector<int> cluster(std::shared_ptr<SequenceFile>& db, const std::shared_ptr<BitVector>& filter);
	void save_edges_external(std::vector<TempFile*> &all_edges,std::vector<TempFile*> &sorted_edges, const std::unordered_map <int64_t, NodEdgSet>& comp, const vector<uint32_t>& s_index);
	vector<int> cluster_sets(const size_t nb_size, vector<TempFile*> &sorted_edges);
	std::unordered_map<int64_t, NodEdgSet> find_connected_components(const std::vector<std::unordered_set<int64_t>> &connected, vector<uint32_t>& EdgSet, const vector<size_t>& nedges);
	vector<TempFile*> mapping_comp_set(std::unordered_map<int64_t, NodEdgSet>& comp);
	void steps(BitVector& current_reps, BitVector& previous_reps, vector<int>& current_centroids, vector<int>& previous_centroids, int count);

public:
	~MultiStep(){};
	void run();
	string get_description();
	static string get_key(){
		return "multi-step";
	}
};

struct Neighbors : public vector<vector<int>>, public Consumer {
	Neighbors(size_t n) : vector<vector<int>>(n), smallest_index(n,0), number_edges(n,0), dSet(n){
		iota(smallest_index.begin(), smallest_index.end(), 0);
	}

	vector<uint32_t> smallest_index;
	vector<size_t> number_edges;
	vector<TempFile*> tempfiles;
	size_t size;	
	LazyDisjointIntegralSet<int64_t> dSet;


	virtual void consume(const char* ptr, size_t n) override {
		const char* end = ptr + n;

		if (config.external) {
			size += n;
			if (tempfiles.empty() || size >= UINT32_MAX) {
				tempfiles.push_back(new TempFile());
				size = 0;
			}
			tempfiles.back()->write(ptr, n);
		}

		while (ptr < end) {
			const uint32_t query = *(uint32_t*)ptr;
			ptr += sizeof(uint32_t);
			const uint32_t subject = *(uint32_t*)ptr;
			ptr += sizeof(uint32_t);

			if (!config.external) {
				(*this)[query].push_back(subject);
			}

			dSet.merge(query, subject);
		
			++number_edges[query];

		}
	}

};
}}
