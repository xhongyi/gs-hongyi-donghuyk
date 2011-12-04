/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: mac
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

#define FRAGMENT_LENGTH 108

using namespace std;

void edit_distribution(string hash_file_name, string ref_file_name,
		string output_file_name, string result_input_name) {
	set_max_indel_num(3);
	set_max_diff_num(3);
	allocatePath();
	ifstream ref_file;
	ifstream input_file;
	ofstream store_file;

	final_result filter_result;
	string testee(FRAGMENT_LENGTH, 'A');
	int monitor_counter = 0; 	// for operation monitoring
	int correct_counter = 0; 	// for operation monitoring
	long long monitor_counter2 = 0; // for operation monitoring
	int gen_coord = 0;
	char test_char;
	long long accumulate_time = 0;

	// get fragment from reference file

	//for (int j = 0 ; j < 1 ; j++) {
	for (int j = 0 ; j < MAX_CONTIG_FILE ; j++) {
		map<int, int> binary_search;
		map<int, int> distribution;
		map<int, int> correct_count;
		char * file_ref	= (char*) malloc(sizeof(char)*50);
		char * file_hash= (char*) malloc(sizeof(char)*50);
		char * file_store= (char*) malloc(sizeof(char)*50);
       		sprintf(file_ref ,  "%s%i", (char*) ref_file_name.c_str(), j);
       		sprintf(file_hash,  "%s%i", (char*) hash_file_name.c_str(), j);
       		sprintf(file_store, "%s%i", (char*) output_file_name.c_str(), j);
		cout << "ref_file_name  :" << file_ref  << endl;
		cout << "hash_file_name :" << file_hash << endl;
		cout << "store_file_name:" << file_store << endl;
		// store file 
		store_file.open(file_store);

		// reference file load at string
		string ref;
		refLoader(ref, file_ref);

		// hash table load 
		cout << "Status : Start load hash table" << endl;
		loadHash(file_hash);
		cout << "Status : End load hash table" << endl;
		ref_file.open(file_ref);
                if (!ref_file.is_open()) {
                        cout << " Error File Open : " << file_ref << endl;
                        break;
                }

		time_t start_time;
		time(&start_time);
		input_file.open(result_input_name.c_str());
		input_file >> testee;
		do {
			filter_result = searchFragment(testee, &ref);
			binary_search[filter_result.total_binary_search]++;
			distribution[filter_result.total_edit_perform]++;
			correct_count[filter_result.total_correct_num]++;
	
			monitor_counter = monitor_counter + 1;
			monitor_counter2 = monitor_counter2 + 1;
			if (monitor_counter >= 10000) {
				fprintf(stdout, "hash distribution count: %lld \n", monitor_counter2);
					monitor_counter = 0;
				}
			if (monitor_counter2 >= 1000000) {
		        	monitor_counter2 = 0;
			}
			input_file >> testee;
		} while (input_file.good());
		
		time_t end_time;
		time(&end_time);
		cout << endl;
		accumulate_time = accumulate_time + difftime(end_time, start_time);
		ref_file.close();
		input_file.close();

		store_file << "Number of Binary Search calculation" << endl;
		long long subsum = 0;
		int       normal = 1;
		for (map<int, int>::iterator p = binary_search.begin(); p != binary_search.end(); p++) {
			//store_file << "index :" << p->first << "	num :" << p->second << endl;
			long long tmp = p->first / normal;
			if (tmp < 10) {
				subsum = subsum + p->second;
			} else if (tmp >= 10) {
				store_file << "index :" << p->first << "	num :" << subsum << endl;
				subsum = 0;
				normal = normal * 10;
			} else {
				store_file << "ERROR" << endl;
			}
		}
		store_file << "index :" << "last"  << "	num :" << subsum << endl;

                long long total_fragment_num0 = 0;
                long long total_binary_num = 0;
                store_file << "Number of Binary Search calculation" << endl;
                for (map<int, int>::iterator p = binary_search.begin(); p != binary_search.end(); p++) {
                        store_file << "index :" << p->first << "        num :" << p->second << endl;
                        total_fragment_num0 = total_fragment_num0 + p->second;
                        total_binary_num = total_binary_num + p->first * p->second;
                }
	
		long long total_fragment_num1 = 0;
		long long total_edit_num = 0;
		store_file << endl << "Number of Edit distance calculation" << endl;
		for (map<int, int>::iterator p = distribution.begin(); p != distribution.end(); p++) {
			store_file << "index :" << p->first << "	num :" << p->second << endl;
			total_fragment_num1 = total_fragment_num1 + p->second;
			total_edit_num = total_edit_num + p->first * p->second;
		}
	
		long long total_fragment_num2 = 0;
		long long total_pass_num = 0;
		store_file << endl << "Number of Edit-distance Passed" << endl;
		for (map<int, int>::iterator p = correct_count.begin(); p != correct_count.end(); p++) {
			store_file << "index :" << p->first << "	num :" << p->second << endl;
			total_fragment_num2 = total_fragment_num2 + p->second;
			total_pass_num = total_pass_num + p->first * p->second;
		}
		store_file << endl;
		store_file << "---------------------------------------------" << endl;
		store_file << "total_binary_num   : " << total_binary_num << endl;
		store_file << "total_fragment_num : " << total_fragment_num1 << endl;
		store_file << "total_edit_num     : " << total_edit_num << endl;
		store_file << "total_pass_num     : " << total_pass_num << endl;
		store_file << "Start_time         : " << ctime(&start_time);
		store_file << "End_time           : " << ctime(&end_time);
		store_file << "TIme Diff          : " << difftime(end_time,start_time) << endl;
		store_file << "Accumulated Time   : " << accumulate_time << endl;
		store_file << "---------------------------------------------" << endl;

		cout << "---------------------------------------------" << endl;
		cout << "total_binary_num   : " << total_binary_num << endl;
		cout << "total_fragment_num : " << total_fragment_num1 << endl;
		cout << "total_edit_num     : " << total_edit_num << endl;
		cout << "total_pass_num     : " << total_pass_num << endl;
		cout << "Start_time         : " << ctime(&start_time);
		cout << "End_time           : " << ctime(&end_time);
		cout << "TIme Diff          : " << difftime(end_time,start_time) << endl;
		cout << "Accumulated Time   : " << accumulate_time << endl;
		cout << "---------------------------------------------" << endl;
		store_file.close();
	}
	cout << "Accumulated Time : " << accumulate_time << endl;
}
