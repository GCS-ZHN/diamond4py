/****
DIAMOND protein aligner
Copyright (C) 2013-2019 Benjamin Buchfink <buchfink@gmail.com>

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

#include <set>
#include <iomanip>
#include "taxonomy_nodes.h"
#include "taxonomy.h"
#include "../util/log_stream.h"
#include "../util/string/string.h"

using namespace std;

void TaxonomyNodes::build(Serializer &out)
{
	task_timer timer("Building taxonomy nodes");
	out.unset(Serializer::VARINT);
	out << taxonomy.parent_;
	out.write_raw(taxonomy.rank_);
	timer.finish();
	message_stream << taxonomy.parent_.size() << " taxonomy nodes processed." << endl;
	size_t rank_count[Rank::count];
	std::fill(rank_count, rank_count + Rank::count, 0);
	for (const Rank r : taxonomy.rank_) {
		++rank_count[r];
	}
	
	const size_t w = MAX_LEN(Rank::names) + 2;
	message_stream << "Number of nodes assigned to rank:" << endl;
	for (size_t i = 0; i < Rank::count; ++i)
		message_stream << std::left << std::setw(w) << Rank::names[i] << rank_count[i] << endl;
	message_stream << endl;
}

TaxonomyNodes::TaxonomyNodes(Deserializer &in, uint32_t db_build)
{
	in.varint = false;
	in >> parent_;
	if (db_build >= 131) {
		rank_.resize(parent_.size());
		in.read(rank_.data(), rank_.size());
	}
	cached_.insert(cached_.end(), parent_.size(), false);
	contained_.insert(contained_.end(), parent_.size(), false);
}

unsigned TaxonomyNodes::get_lca(unsigned t1, unsigned t2) const
{
	static const int max = 64;
	if (t1 == t2 || t2 == 0)
		return t1;
	if (t1 == 0)
		return t2;
	unsigned p = t2;
	set<unsigned> l;
	l.insert(p);
	int n = 0;
	do {
		p = get_parent(p);
		if (p == 0)
			return t1;
		l.insert(p);
		if (++n > max)
			throw std::runtime_error("Path in taxonomy too long (1).");
	} while (p != t1 && p != 1);
	if (p == t1)
		return p;
	p = t1;
	n = 0;
	while (l.find(p) == l.end()) {
		p = get_parent(p);
		if (p == 0)
			return t2;
		if (++n > max)
			throw std::runtime_error("Path in taxonomy too long (2).");
	}
	return p;
}

bool TaxonomyNodes::contained(unsigned query, const set<unsigned> &filter)
{
	static const int max = 64;
	if (query >= parent_.size())
		throw runtime_error(string("No taxonomy node found for taxon id ") + to_string(query));
	if (cached_[query])
		return contained_[query];
	if (filter.find(1) != filter.end())
		return true;
	int n = 0;
	unsigned p = query;
	while (p > 1 && filter.find(p) == filter.end()) {
		p = get_parent(p);
		if (++n > max)
			throw std::runtime_error("Path in taxonomy too long (3).");
	}
	const bool contained = p > 1;
	unsigned q = query;
	while (set_cached(q, contained), q != p)
		q = get_parent(q);
	return contained;
}

bool TaxonomyNodes::contained(const vector<unsigned> query, const set<unsigned> &filter)
{
	static const int max = 64;
	if (filter.find(1) != filter.end())
		return true;
	for (vector<unsigned>::const_iterator i = query.begin(); i != query.end(); ++i)
		if (contained(*i, filter))
			return true;
	return false;
}

unsigned TaxonomyNodes::rank_taxid(unsigned taxid, Rank rank) const {
	static const int max = 64;
	int n = 0;
	while (true) {
		if (taxid >= rank_.size())
			return 0;
		if (rank_[taxid] == rank)
			return taxid;
		if (taxid == 0 || taxid == 1)
			return 0;
		if (++n > max)
			throw std::runtime_error("Path in taxonomy too long (4).");
		taxid = get_parent(taxid);
	}
	return 0;
}

std::set<unsigned> TaxonomyNodes::rank_taxid(const std::vector<unsigned> &taxid, Rank rank) const {
	set<unsigned> r;
	for (unsigned i : taxid)
		r.insert(rank_taxid(i, rank));
	return r;
}