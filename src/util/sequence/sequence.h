/****
DIAMOND protein aligner
Copyright (C) 2016-2021 Max Planck Society for the Advancement of Science e.V.
                        Benjamin Buchfink
						
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

#pragma once
#include <string>
#include <string.h>
#include "../../basic/sequence.h"
#include "../io/output_file.h"

namespace Util { namespace Seq {

void format(Sequence seq, const char *id, const char *qual, OutputFile &out, const std::string &format, const ValueTraits& value_traits);

static inline Sequence clip(const Letter *seq, int len, int anchor) {
	const Letter *a = seq + anchor, *begin = seq, *end = seq + len, *p;
	for(;;) {
		p = (const Letter*)memchr(begin, (int)Sequence::DELIMITER, end - begin);
		if (p == nullptr)
			return Sequence(begin, end);
		if (p >= a)
			return Sequence(begin, p);
		begin = p + 1;
	}
}

extern const char* id_delimiters;

std::string all_seqids(const char* s);
std::string seqid(const char* title, bool short_seqids);
void get_title_def(const std::string& s, std::string& title, std::string& def);
bool is_fully_masked(const Sequence& seq);

}}
