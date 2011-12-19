/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: mac
 */
#include "ref_db_generator.h"
#include "fragment_match.h"
#include "prefil_distribution.h"

#define FRAGMENT_LENGTH 108

using namespace std;

void prefil_distribution(string hash_file_name, string ref_file_name,
		string output_file_name) {
	set_max_indel_num(3);
	set_max_diff_num(3);
	ifstream ref_file;
	ofstream store_file;
	ref_file.open(ref_file_name.c_str());
	store_file.open(output_file_name.c_str());
	GPU_data result;
	long long AF_distribution[32];
	long long ED_distribution[32];
	for (int i = 0; i < 32; i++) {
		AF_distribution[i] = 0;
		ED_distribution[i] = 0;
	}
	string testee(FRAGMENT_LENGTH, 'A');
	int monitor_counter = 0; // for operation monitoring
	long long monitor_counter2 = 0; // for operation monitoring
	char test_char;
	cout << "Status : Start load hash table" << endl;
	loadHash(hash_file_name.c_str());
	cout << "Status : End load hash table" << endl;
	do {
		ref_file >> test_char;
	} while (test_char == 'N');
	testee[0] = test_char;
	for (int i = 1; i < FRAGMENT_LENGTH; i++) {
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
			for (int i = 0; i < FRAGMENT_LENGTH; i++) {
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
			testee = testee.substr(1, FRAGMENT_LENGTH - 1) + test_char;
		}

		result = binary_filtering_GPU_cost(testee);

		AF_distribution[0] += result.AF_count / 32;
		AF_distribution[result.AF_count % 32]++;
		ED_distribution[0] += result.ED_count / 32;
		ED_distribution[result.ED_count % 32]++;
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

	} while (ref_file.good());

	for (int i = 0; i < 32; i++) {
		store_file << "AF_warp: " << i << "	# :" << AF_distribution[i] << endl;
	}

	store_file << "----------------------------ED distribution----------------------------" << endl;

	for (int i = 0; i < 32; i++) {
		store_file << "ED_warp: " << i << "	# :" << ED_distribution[i] << endl;
	}

	ref_file.close();
	store_file.close();
}

