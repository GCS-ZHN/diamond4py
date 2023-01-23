/****
DIAMOND protein aligner
Copyright (C) 2013-2017 Benjamin Buchfink <buchfink@gmail.com>

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

#include <iostream>
#include "main.h"

using std::cout;
using std::cerr;
using std::endl;

void model_seqs();
void opt();
void run_masker();
void fastq2fasta();
void view_daa();
void view_tsv();
void db_info();
void test_main();
void benchmark_sw();
void db_annot_stats();
void read_sim();
void info();
void seed_stat();
void pairwise();
void translate();
void filter_blasttab();
void show_cbs();
void reverse();
void get_medoids_from_tree();
void roc();
void merge_tsv();
void roc_id();
void makeindex();
void find_shapes();
void composition();
void join();
void hash_seqs();
void list_seeds();
void merge_daa();
#ifdef WITH_BLASTDB
void prep_blast_db();
#endif

void split();
namespace Benchmark { DECL_DISPATCH(void, benchmark, ()) }
namespace Util { namespace Algo { namespace UPGMA { void upgma(); } } }
namespace Util { namespace Algo { namespace UPGMA_MC { void upgma(); } } }
namespace Test { int run();
void simulate_seqs();
void mutate();
}

int diamond(int ac, const char* av[])
{
	config = Config(ac, av);

	switch (config.command) {
	case Config::help:
		break;
	case Config::version:
		cout << Const::program_name << " version " << Const::version_string << endl;
		break;
	case Config::makedb:
		DatabaseFile::make_db();
		break;
	case Config::blastp:
	case Config::blastx:
		Search::run();
		break;
	case Config::view:
		if (!config.daa_file.empty())
			view_daa();
		else if (!config.input_ref_file.empty())
			view_tsv();
		else
			throw std::runtime_error("The view command requires either a text (option --in) or DAA (option -a) input file.");
		break;
	case Config::getseq:
		get_seq();
		break;
	case Config::random_seqs:
		random_seqs();
		break;
	case Config::sort:
		sort_file();
		break;
	case Config::db_stat:
		db_stat();
		break;
	case Config::mask:
		run_masker();
		break;
	case Config::fastq2fasta:
		fastq2fasta();
		break;
	case Config::dbinfo:
		db_info();
		break;
	case Config::read_sim:
		read_sim();
		break;
	case Config::info:
		info();
		break;
	case Config::smith_waterman:
		pairwise();
		break;
	case Config::cluster:
		// Why is cluster_similarity not set at the end of the Config constructor?
		if(!config.cluster_similarity.empty()){
			string expression = RecursiveParser::clean_expression(&config.cluster_similarity);
			RecursiveParser rp(nullptr, expression.c_str(), true);
			try{
				rp.evaluate();
			}
			catch (const runtime_error& e){
				message_stream << "Could not evaluate the expression: " << config.cluster_similarity << endl;
				throw e;
			}
		}
		Workflow::Cluster::ClusterRegistry::get(config.cluster_algo)->run();
		break;
	case Config::translate:
		translate();
		break;
	case Config::filter_blasttab:
		filter_blasttab();
		break;
	case Config::show_cbs:
		show_cbs();
		break;
	case Config::simulate_seqs:
		Test::simulate_seqs();
		break;
	case Config::benchmark:
		Benchmark::benchmark();
		break;
	case Config::split:
		split();
		break;
	case Config::upgma:
		Util::Algo::UPGMA::upgma();
		break;
	case Config::upgma_mc:
		Util::Algo::UPGMA_MC::upgma();
		break;
	case Config::regression_test:
		return Test::run();
		break;
	case Config::reverse_seqs:
		reverse();
		break;
	case Config::compute_medoids:
		get_medoids_from_tree();
		break;
	case Config::mutate:
		Test::mutate();
		break;
	case Config::roc:
		roc();
		break;
	case Config::merge_tsv:
		merge_tsv();
		break;
	case Config::rocid:
		roc_id();
		break;
	case Config::makeidx:
		makeindex();
		break;
	case Config::find_shapes:
		find_shapes();
		break;
	case Config::HASH_SEQS:
		hash_seqs();
		break;
#ifdef WITH_BLASTDB
	case Config::prep_blast_db:
		prep_blast_db();
		break;
#endif
	case Config::composition:
		composition();
		break;
	case Config::JOIN:
		join();
		break;
	case Config::LIST_SEEDS:
		list_seeds();
		break;
	case Config::MERGE_DAA:
		merge_daa();
		break;
	default:
		return 1;
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	try {
		return diamond(argc, argv);
	}
	catch(const std::bad_alloc &e) {
		cerr << "Failed to allocate sufficient memory. Please refer to the manual for instructions on memory usage." << endl;
		log_stream << "Error: " << e.what() << endl;
		return 1;
	} catch(const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        log_stream << "Error: " << e.what() << endl;
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!" << endl;
        return 1;
    }
}

