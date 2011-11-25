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
	map<int, int> distribution;
	map<int, int> correct_count;
	string testee(FRAGMENT_LENGTH, 'A');
	int monitor_counter = 0; 	// for operation monitoring
	int correct_counter = 0; 	// for operation monitoring
	long long monitor_counter2 = 0; // for operation monitoring
	int gen_coord = 0;
	char test_char;
	double accumulate_time = 0;

	// get fragment from reference file
	store_file.open(output_file_name.c_str());

	for (int j = 0 ; j < MAX_CONTIG_FILE ; j++) {
		char * file_ref	= (char*) malloc(sizeof(char)*50);
		char * file_hash= (char*) malloc(sizeof(char)*50);
       		sprintf(file_ref , "%s%i", (char*) ref_file_name.c_str(), j);
       		sprintf(file_hash, "%s%i", (char*) hash_file_name.c_str(), j);
		cout << "ref_file_name :" << file_ref  << endl;
		cout << "hash_file_name:" << file_hash << endl;

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
			distribution[filter_result.total_edit_perform]++;
			correct_count[filter_result.total_correct_num]++;
	
			monitor_counter = monitor_counter + 1;
			monitor_counter2 = monitor_counter2 + 1;
			if (monitor_counter >= 10000) {
				fprintf(stdout, "hash distribution count: %lld \n", monitor_counter2);
					monitor_counter = 0;
				}
			if (monitor_counter2 >= 1000000) {
		        //	monitor_counter2 = 0;
			//	break;
			}
			input_file >> testee;
		} while (input_file.good());
		
		time_t end_time;
		time(&end_time);
		cout << "Start_time: " << ctime(&start_time) << endl;;
		cout << "End_time  : " << ctime(&end_time) << endl;
		cout << "TIme Diff : " << difftime(end_time,start_time) << endl;
		accumulate_time = accumulate_time + difftime(end_time, start_time);
		ref_file.close();
		store_file.close();
		input_file.close();
	}
	long long total_fragment_num = 0;
	long long total_perform_num = 0;
	store_file << "Binary Search Result" << endl;
	for (map<int, int>::iterator p = distribution.begin(); p
			!= distribution.end(); p++) {
		store_file << "index :" << p->first << "	# :" << p->second << endl;
		total_fragment_num = total_fragment_num + p->second;
		total_perform_num = total_perform_num + p->first * p->second;
	}
	store_file << "total_fragment_num :" << total_fragment_num << endl;
	store_file << "total_perform_num  :" << total_perform_num << endl;
	store_file << endl << "Edit-distance Search Result" << endl;
	for (map<int, int>::iterator p = correct_count.begin(); p
			!= correct_count.end(); p++) {
		store_file << "index :" << p->first << "	# :" << p->second << endl;
	}
	cout << "Total Time : " << accumulate_time << endl;
}
