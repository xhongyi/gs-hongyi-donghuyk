/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *	  Author: mac
 */
#include <iostream>
#include <deque>
#include <list>
#include <map>
#include <cstdio>
#include <fstream>
#include <string>
#include <time.h>
#include "common.h"

#include "ref_db_generator.h"
#include "fragment_match.h"
#include "edit_distance.h"
#include "edit_distribution.h"
#include "fragment_match.h"

using namespace std;

void edit_normal(string hash_file_name, string mask_file_name,
		string ref_file_name, string output_file_name,
		string result_input_name) {
	allocatePath();
	ifstream ref_file;
	ifstream input_file;
	//ifstream contig_name_file;
	ofstream store_file;
	ofstream output_file;

	final_result filter_result;
	string testee_name(fragment_length_, 'A');
	string testee_seq(fragment_length_, 'A');
	string testee_dummy(fragment_length_, 'A');
	string testee_qual(fragment_length_, 'A');
	int monitor_counter1 = 0; // for operation monitoring
	int correct_counter = 0; // for operation monitoring
	long long monitor_counter2 = 0; // for operation monitoring
	int gen_coord = 0;
	char test_char;
	long long acc_cal_time = 0;
	long long acc_load_time = 0;
	long long acc_total_time = 0;

	char file_contig[50];
	char contig_name[MAX_CONTIG_FILE][100];
	sprintf(file_contig, "%s%s", (char*) ref_file_name.c_str(), "name");
	//contig_name_file.open(file_contig);
	/*
	for (int i = 0; i < MAX_CONTIG_FILE; i++) {
		contig_name_file.getline(contig_name[i], 100);
		//cout << "i:" << i << "   " << contig_name[i] << endl;
	}
	*/

	char file_output[50];
	sprintf(file_output, "%s%s", (char*) output_file_name.c_str(), "sam");
	cout << "output_file_name:" << file_output << endl;
	output_file.open(file_output);

// get fragment from reference file
//	for (int j = MAX_CONTIG_FILE - 1 ; j < MAX_CONTIG_FILE ; j++) {
//	for (int j = 0 ; j < 4 ; j++) {
	for (int j = 0; j < 1/*MAX_CONTIG_FILE*/; j++) {
		time_t start_load_time;
		time(&start_load_time);
		map<int, int> binary_search;
		map<int, int> binary_success;
		map<int, int> distribution;
		map<int, int> correct_count;
		map<int, int> bit_mask_success;
		map<int, int> filtering;
		char file_ref[50];
		char file_hash[50];
		char file_store[50];
		char file_mask[50];
		sprintf(file_ref, "%s%i", (char*) ref_file_name.c_str(), j);
		sprintf(file_hash, "%s%i", (char*) hash_file_name.c_str(), j);
		sprintf(file_store, "%s%i", (char*) output_file_name.c_str(), j);
		sprintf(file_mask, "%s%i", (char*) mask_file_name.c_str(), j);
		cout << "ref_file_name  :" << file_ref << endl;
		cout << "hash_file_name :" << file_hash << endl;
		cout << "store_file_name:" << file_store << endl;
		cout << "mask_file_name :" << file_mask << endl;

		// store file 
		store_file.open(file_store);

		// reference file load at string
		string ref;
		refLoader(ref, file_ref);

		// hash table load 
		cout << "Status : Start load hash table" << endl;
		loadHash(file_hash, j);
		cout << "Status : End load hash table" << endl;
		ref_file.open(file_ref);
		if (!ref_file.is_open()) {
			cout << " Error File Open : " << file_ref << endl;
			break;
		}
		time_t end_load_time;
		time(&end_load_time);

		time_t start_cal_time;
		time(&start_cal_time);
		// Add reverse complement mode
		for (int k = 0; k < 2; k++) {
			if (k == 0) {
				cout << "Normal  Mode" << endl;
				set_reverse_mode(false);
			} else if (k == 1) {
				cout << "Reverse Mode" << endl;
				set_reverse_mode(true);
			}
			input_file.open(result_input_name.c_str());
			input_file >> testee_name;
			input_file >> testee_seq;
			input_file >> testee_dummy;
			input_file >> testee_qual;
//				cout << "original:" << testee_seq << endl;
			if (reverse_mode == true) {
				char * dummy = (char*) malloc(sizeof(char) * testee_seq.size());
				reverseComplete(testee_seq.c_str(), dummy, testee_seq.size());
				testee_seq = string(dummy);
				free(dummy);
//				cout << "Reverse :" << testee_seq << endl;
			}
			do {
				filter_result = searchFragment_fastq(testee_seq, &ref,
						&output_file, contig_name[j], testee_name.substr(1),
						testee_qual);
				bit_mask_success[filter_result.total_bit_mask_success]++;
				filtering[filter_result.total_filtering]++;
				binary_search[filter_result.total_binary_search]++;
				binary_success[filter_result.total_binary_success]++;
				distribution[filter_result.total_edit_perform]++;
				correct_count[filter_result.total_correct_num]++;

				monitor_counter1 = monitor_counter1 + 1;
				monitor_counter2 = monitor_counter2 + 1;
				if (monitor_counter1 >= 10000) {
					fprintf(stdout, "hash distribution count: %lld \n",
							monitor_counter2);
					monitor_counter1 = 0;
				}
				if (monitor_counter2 >= 1000000) {
					monitor_counter2 = 0;
				}
				input_file >> testee_name;
				input_file >> testee_seq;
				input_file >> testee_dummy;
				input_file >> testee_qual;
			} while (input_file.good());
			input_file.close();
		}
		cout << endl;

		time_t end_cal_time;
		time(&end_cal_time);

		acc_load_time = acc_load_time
				+ difftime(end_load_time, start_load_time);
		acc_cal_time = acc_cal_time + difftime(end_cal_time, start_cal_time);
		acc_total_time = acc_total_time
				+ difftime(end_cal_time, start_load_time);
		ref_file.close();

		long long subsum = 0;
		int normal = 1;

		store_file << "Number of Filtering" << endl;
		for (map<int, int>::iterator iter = filtering.begin();
				iter != filtering.end(); iter++) {
			long long tmp = iter->first / normal;
			if (tmp < 10) {
				subsum += iter->second;
			} else if (tmp >= 10) {
				store_file << "index: " << iter->first << "	num: " << subsum
						<< endl;
				subsum = 0;
				normal = normal * 10;
			} else {
				store_file << "ERROR" << endl;
			}
		}
		store_file << "index: last	num: " << subsum << endl;

		long long total_filtering = 0;
		store_file << "Number of Filtering" << endl;
		for (map<int, int>::iterator p = filtering.begin();
				p != filtering.end(); p++) {
			store_file << "index :" << p->first << "		num :" << p->second
					<< endl;
			total_filtering += p->first * p->second;
		}

		long long total_bit_mask_success = 0;
		store_file << "Number of Bit_mask_success" << endl;
		for (map<int, int>::iterator p = bit_mask_success.begin();
				p != bit_mask_success.end(); p++) {
			store_file << "index :" << p->first << "		num :" << p->second
					<< endl;
			total_bit_mask_success += p->first * p->second;
		}

		store_file << "Number of Binary Search calculation" << endl;
		subsum = 0;
		normal = 1;
		for (map<int, int>::iterator p = binary_search.begin();
				p != binary_search.end(); p++) {
			//store_file << "index :" << p->first << "	num :" << p->second << endl;
			long long tmp = p->first / normal;
			if (tmp < 10) {
				subsum = subsum + p->second;
			} else if (tmp >= 10) {
				store_file << "index :" << p->first << "	num: " << subsum
						<< endl;
				subsum = 0;
				normal = normal * 10;
			} else {
				store_file << "ERROR" << endl;
			}
		}
		store_file << "index: last	num: " << subsum << endl;

		long long total_binary_num = 0;
		store_file << "Number of Binary Search calculation" << endl;
		for (map<int, int>::iterator p = binary_search.begin();
				p != binary_search.end(); p++) {
			store_file << "index :" << p->first << "		num :" << p->second
					<< endl;
			total_binary_num = total_binary_num + p->first * p->second;
		}

		store_file << "Number of Binary Search success" << endl;
		subsum = 0;
		normal = 1;
		for (map<int, int>::iterator p = binary_success.begin();
				p != binary_success.end(); p++) {
			//store_file << "index :" << p->first << "	num :" << p->second << endl;
			long long tmp = p->first / normal;
			if (tmp < 10) {
				subsum = subsum + p->second;
			} else if (tmp >= 10) {
				store_file << "index :" << p->first << "	num: " << subsum
						<< endl;
				subsum = 0;
				normal = normal * 10;
			} else {
				store_file << "ERROR" << endl;
			}
		}
		store_file << "index: last	num: " << subsum << endl;

		long long total_binary_success = 0;
		store_file << "Number of Binary Search success" << endl;
		for (map<int, int>::iterator p = binary_success.begin();
				p != binary_success.end(); p++) {
			store_file << "index: " << p->first << "		num: " << p->second
					<< endl;
			total_binary_success += p->first * p->second;
		}

		long long total_fragment_num = 0;
		long long total_edit_num = 0;
		store_file << endl << "Number of Edit distance calculation" << endl;
		for (map<int, int>::iterator p = distribution.begin();
				p != distribution.end(); p++) {
			store_file << "index :" << p->first << "	num :" << p->second
					<< endl;
			total_fragment_num = total_fragment_num + p->second;
			total_edit_num = total_edit_num + p->first * p->second;
		}

		long long total_pass_num = 0;
		store_file << endl << "Number of Edit-distance Passed" << endl;
		for (map<int, int>::iterator p = correct_count.begin();
				p != correct_count.end(); p++) {
			store_file << "index :" << p->first << "	num :" << p->second
					<< endl;
			total_pass_num = total_pass_num + p->first * p->second;
		}
		store_file << endl;
		store_file << "---------------------------------------------" << endl;
		store_file << "total_filtering_____: " << total_filtering << endl;
		store_file << "total_binary_num____: " << total_binary_num << endl;
		store_file << "total_binary_success: " << total_binary_success << endl;
		store_file << "total_bit_mask_sucess:" << total_bit_mask_success
				<< endl;
		store_file << "total_fragment_num__: " << total_fragment_num << endl;
		store_file << "total_edit_num______: " << total_edit_num << endl;
		store_file << "total_pass_num______: " << total_pass_num << endl;
		store_file << "---------------------------------------------" << endl;
		store_file << "Time_load_hash______: "
				<< difftime(end_load_time, start_load_time) << endl;
		store_file << "TIme_Calulation_____: "
				<< difftime(end_cal_time, start_cal_time) << endl;
		store_file << "Time_Total__________: "
				<< difftime(end_cal_time, start_load_time) << endl;
		store_file << "---------------------------------------------" << endl;
		store_file << "Acc_load__Time______: " << acc_load_time << endl;
		store_file << "Acc_Cal___Time______: " << acc_cal_time << endl;
		store_file << "Acc_Total_Time______: " << acc_total_time << endl;
		store_file << "---------------------------------------------" << endl;

		cout << "---------------------------------------------" << endl;
		cout << "total_binary_num____: " << total_binary_num << endl;
		cout << "total_fragment_num__: " << total_fragment_num << endl;
		cout << "total_edit_num______: " << total_edit_num << endl;
		cout << "total_pass_num______: " << total_pass_num << endl;
		cout << "---------------------------------------------" << endl;
		cout << "Time_load_hash______: "
				<< difftime(end_load_time, start_load_time) << endl;
		cout << "TIme_Calulation_____: "
				<< difftime(end_cal_time, start_cal_time) << endl;
		cout << "Time_Total__________: "
				<< difftime(end_cal_time, start_load_time) << endl;
		cout << "---------------------------------------------" << endl;
		cout << "Acc_load__Time______: " << acc_load_time << endl;
		cout << "Acc_Cal___Time______: " << acc_cal_time << endl;
		cout << "Acc_Total_Time______: " << acc_total_time << endl;
		cout << "---------------------------------------------" << endl;

		for (int i = 0; i < 7; i++) {
			store_file << i + 1 << " level bit mask success: " << "\t" <<
					bm.get_eval_data(true, i) << endl;
			store_file << i + 1 << " level bit mask fail: " << "\t\t" <<
					bm.get_eval_data(false, i) << endl;
		}
		store_file.close();
		freeHash();
		monitor_counter1 = 0;
		monitor_counter2 = 0;
	}
	cout << "Acc_load__Time: " << acc_load_time << endl;
	cout << "Acc_Cal___Time: " << acc_cal_time << endl;
	cout << "Acc_Total_Time: " << acc_total_time << endl;
	output_file.close();
	deallocatePath();
}
