/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: mac
 */
#include "ref_db_generator.h"
#include "fragment_match.h"
#include "prefil_distribution.h"

using namespace std;

void prefil_distribution(string hash_file_name, string ref_file_name,
		string output_file_name) {
	set_max_indel_num(3);
	set_max_diff_num(3);
	ifstream ref_file;
	ofstream store_file;
	ref_file.open(ref_file_name.c_str());
	store_file.open(output_file_name.c_str());
	int result ;
	map< int , int> distribution;
	string testee(fragment_length_, 'A');
	int monitor_counter = 0; // for operation monitoring
	long long monitor_counter2 = 0; // for operation monitoring
	char test_char;
	cout << "Status : Start load hash table" << endl;
	loadHash(hash_file_name.c_str());
	cout << "Status : End load hash table" << endl;
	do {
		ref_file >> test_char;
	}while (test_char == 'N');
	testee[0] = test_char;
	for (int i = 1; i < fragment_length_; i++) {
		ref_file >> test_char;
		if (test_char == 'N') {
			i = 0;
			ref_file >> test_char;
		} else
		testee[i] = test_char;
	}
	do {
		//cout << "string: "<< testee << endl;
		ref_file >> test_char;
		if (test_char == 'N') {
			for (int i = 0; i < fragment_length_; i++) {
				ref_file >> test_char;
				if (test_char == 'N') {
					i = 0;
					ref_file >> test_char;
				}
				if (test_char != 'N') {
					testee[i] = test_char;
				}
				if (!ref_file.good())
				break;
			}
		} else {
			testee = testee.substr(1, fragment_length_ - 1) + test_char;
		}

		result = binary_filtering_cost(testee);

		distribution[result]++;
		//		cout <<" result: " << result.size() << endl;
		monitor_counter = monitor_counter + 1;
		monitor_counter2 = monitor_counter2 + 1;
		//cout <<" monitor: " << monitor_counter << endl;

		if (monitor_counter >= 1000000) {
			fprintf(stdout, "hash distribution count: %lld \n",
					monitor_counter2);
			monitor_counter = 0;
			break;
		}

	}while (ref_file.good());
	long long total_fragment_num = 0;
	long long total_perform_num = 0;
	for (map<int, int>::iterator p = distribution.begin(); p
			!= distribution.end(); p++) {
		store_file << "index :" << p->first << "	# :" << p->second << endl;
		total_fragment_num = total_fragment_num + p->second;
		total_perform_num = total_perform_num + p->first * p->second;
	}
	store_file << "total_fragment_num :" << total_fragment_num << endl;
	store_file << "total_perform_num  :" << total_perform_num << endl;

	ref_file.close();
	store_file.close();
}

