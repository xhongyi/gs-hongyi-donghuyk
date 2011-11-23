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
#include "common.h"

#include "ref_db_generator.h"
#include "fragment_match.h"
#include "edit_distance.h"
#include "edit_distribution.h"

#define FRAGMENT_LENGTH 108

int* hash_table;
int* coordinate;

using namespace std;

void loadHash(string hash_name);
 
void loadHash(string hash_name) {
        hashReconstructor(&hash_table, &coordinate, hash_name.c_str());
}

void edit_distribution(string hash_file_name, string ref_file_name, string output_file_name) {
	set_max_indel_num(3);
	set_max_diff_num(3);
	allocatePath();
	ifstream ref_file;
	ofstream store_file;
	// reference file load at string
	string ref;
	ref_file.open(ref_file_name.c_str()); 
	refLoader(ref, (char*)ref_file_name.c_str());
	ref_file.close();
	// get fragment from reference file
	ref_file.open(ref_file_name.c_str()); 
	store_file.open(output_file_name.c_str()); 
	map <int, int> distribution;
	map <int, int> distribution2;
	string testee(FRAGMENT_LENGTH, 'A');
	int monitor_counter = 0;		// for operation monitoring
	int correct_counter = 0;		// for operation monitoring
	long long monitor_counter2 = 0;       	// for operation monitoring
	int dist_count = 0;
	int gen_coord = 0;
	char test_char;
	cout << "Status : Start load hash table" << endl;
	loadHash(hash_file_name.c_str());
	cout << "Status : End load hash table" << endl;

	do {
		ref_file >> test_char;
		gen_coord = gen_coord + 1;
	} while (test_char == 'N');
	testee[0] = test_char;
	for (int i = 1; i < FRAGMENT_LENGTH; i++) {
		ref_file >> test_char;
		gen_coord = gen_coord + 1;
		if (test_char == 'N') {
			i = 0;
			ref_file >> test_char;
			gen_coord = gen_coord + 1;
		} else 
		testee[i] = test_char;
	}
	do {	
		ref_file >> test_char;
		gen_coord = gen_coord + 1;
		if (test_char == 'N') {
			for (int i = 0; i < FRAGMENT_LENGTH; i++) {
       			 	ref_file >> test_char;
				gen_coord = gen_coord + 1;
				if (test_char == 'N'){ 
					i = 0;
					ref_file >> test_char;
					gen_coord = gen_coord + 1;
				}
				if (test_char != 'N') {
					testee[i] = test_char;
				}
				if (!ref_file.good()) break;
       		 	}
		} else {
			testee = testee.substr(1, FRAGMENT_LENGTH - 1) + test_char;
		}

		cout << "testee_coordinate:"  << gen_coord - READ_LENGTH << "  "<< endl;
		cout << "testee_read      : " << testee << endl;

	        key_struct keys_input[max_diff_num + 1];
	        list<match_result> result;
		for (int i = 0; i < KEY_NUMBER; i++) {
			string key = testee.substr(KEY_LENGTH * i, KEY_LENGTH);
			int key_hash = hashVal(key);
			int key_entry = hash_table[key_hash];
			int key_entry_size = coordinate[key_entry];
			keys_input[i].order = 0;
			keys_input[i].key_number = i;
			keys_input[i].key_entry = key_entry;
			keys_input[i].key_entry_size = key_entry_size;
		}
		for (int k = 0; k < max_diff_num + 1; k++) {
			for (int i = keys_input[k].key_entry + 1; i <= keys_input[k].key_entry + keys_input[k].key_entry_size; i++) {
				int coor_value = coordinate[i] - keys_input[k].key_number*k;
				string ref_str(FRAGMENT_LENGTH, 'A'); 
				ref_str =  ref.substr(coor_value, FRAGMENT_LENGTH);
				ED_result edit_result = editDistanceCal(ref_str, testee);
				if (edit_result.correct) {
					correct_counter = correct_counter+1;
					cout << "ref_read	 : " << ref_str << "  coordinate: "<< coor_value << "  Key_number: "<< keys_input[k].key_number;
					cout << "  result: correct " << endl;
				} else {
					cout << "ref_read	 : " << ref_str << "  coordinate: "<< coor_value << "  Key_number: "<< keys_input[k].key_number;
					cout << "  result: not correct" <<endl;
				}	
				dist_count = dist_count+1;
			}
		}
cout << "TEST 7" << endl;
		distribution2[correct_counter]++;
cout << "TEST 8" << endl;
		distribution[dist_count]++;
		dist_count = 0;
		correct_counter = 0;

		monitor_counter = monitor_counter + 1;
		monitor_counter2 = monitor_counter2 + 1;
                if (monitor_counter >= 1000) {
                        fprintf(stdout, "hash distribution count: %lld \n", monitor_counter2);
                        monitor_counter = 0;
                }
                if (monitor_counter2 >= 1000000){
                        break;
                }
	} while (ref_file.good());
cout << "TEST 6" << endl;
	long long total_fragment_num = 0;	
	long long total_perform_num = 0;	
	store_file << "Binary Search Result" << endl;
	for (map<int,int>::iterator p = distribution.begin(); p != distribution.end() ; p++) {
		store_file <<"index :"<< p->first << "	# :" << p->second << endl; 
		total_fragment_num = total_fragment_num + p->second;
		total_perform_num  = total_perform_num + p->first*p->second;
	}
	store_file <<"total_fragment_num :"<< total_fragment_num << endl;
	store_file <<"total_perform_num  :"<< total_perform_num  << endl;
	store_file << endl << "Edit-distance Search Result" << endl;
	for (map<int,int>::iterator q = distribution2.begin(); q != distribution2.end() ; q++) {
		store_file <<"index :"<< q->first << "	# :" << q->second << endl; 
	}

cout << "TEST 7" << endl;
	ref_file.close();
cout << "TEST 8" << endl;
	store_file.close();
cout << "TEST 9" << endl;
}
