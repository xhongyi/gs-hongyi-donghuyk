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
#include "common.h"

#include "ref_db_generator.h"
#include "fragment_match.h"
#include "edit_distance.h"

#define FRAGMENT_LENGTH 108

using namespace std;

void edit_distribution(string hash_file_name, string ref_file_name, string ref2_file_name, string output_file_name) {
	set_max_indel_num(5);
	set_max_diff_num(5);
        allocatePath();
	ifstream ref_file;
	ofstream store_file;
	ref_file.open(ref_file_name.c_str()); 
	store_file.open(output_file_name.c_str()); 
	list<match_result> filter_result;
	map <int, int> distribution;
	map <int, int> correct_count;
        string testee(FRAGMENT_LENGTH, 'A');
        int monitor_counter = 0;        	// for operation monitoring
        int correct_counter = 0;        	// for operation monitoring
        long long monitor_counter2 = 0;       	// for operation monitoring
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
		//cout << "string: "<< testee << endl;
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

		filter_result = searchFragment(testee);

		cout << "testee_coordinate:"  << gen_coord - READ_LENGTH << "  "<< endl;
		cout << "testee_read      : " << testee << endl;

		for(list<match_result>::iterator it_result=filter_result.begin(); it_result != filter_result.end(); ++it_result) {
			string ref_str(FRAGMENT_LENGTH, 'A'); 
			ref_str =  getRefSeq((*it_result).coordinate, FRAGMENT_LENGTH, ref2_file_name);
			ED_result edit_result = editDistanceCal(ref_str, testee);
			if (edit_result.correct) {
				correct_counter = correct_counter+1;
				cout << "ref_read         : " << ref_str << "  coordinate: "<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
				cout << "  result: correct " << endl;
			} else {
				cout << "ref_read         : " << ref_str << "  coordinate: "<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
				cout << "  result: not correct" <<endl;
			}
		}
		distribution[filter_result.size()]++;
		correct_count[correct_counter]++;
		correct_counter = 0;

		monitor_counter = monitor_counter + 1;
		monitor_counter2 = monitor_counter2 + 1;
		if (monitor_counter >= 1000) {
			fprintf(stdout, "hash distribution count: %lld \n", monitor_counter2);
			monitor_counter = 0;
		}
//	break;
	} while (ref_file.good());
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
	for (map<int,int>::iterator p = correct_count.begin(); p != correct_count.end() ; p++) {
		store_file <<"index :"<< p->first << "	# :" << p->second << endl; 
	}

	ref_file.close();
	store_file.close();
}