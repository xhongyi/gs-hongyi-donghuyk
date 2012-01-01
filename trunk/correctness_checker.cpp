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
#include "correctness_checker.h"
#include "edit_distance.h"

using namespace std;

void correctness_checker(string hash_file_name, string ref_file_name, string ref2_file_name, string output_file_name);

void correctness_checker(string hash_file_name, string ref_file_name, string ref2_file_name, string output_file_name) {
	set_max_indel_num(3);
	set_max_diff_num(3);
        allocatePath();
	ifstream ref_file;
	ofstream store_file;
	ref_file.open(ref_file_name.c_str()); 
	store_file.open(output_file_name.c_str()); 
	list<match_result> first_filter_result;
	list<match_result> cheap_filter_result;
        string testee(fragment_length_, 'A');
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
        for (int i = 1; i < fragment_length_; i++) {
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
	        	for (int i = 0; i < fragment_length_; i++) {
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
			testee = testee.substr(1, fragment_length_ - 1) + test_char;
		}

		set_select_cheapest(false);
		first_filter_result = searchFragment(testee);
		set_select_cheapest(true);
		cheap_filter_result = searchFragment(testee);

		cout << "testee_coordinate:"  << gen_coord - fragment_length_ << "  "<< endl;
		cout << "testee_read      : " << testee << endl;

		for(list<match_result>::iterator it_result = first_filter_result.begin(); it_result != first_filter_result.end(); ++it_result) {
			string ref_str(fragment_length_, 'A'); 
			ref_str =  getRefSeq((*it_result).coordinate, fragment_length_, ref2_file_name);
			ED_result edit_result = editDistanceCal(ref_str, testee);
			if (edit_result.correct) {
				it_result->correct = true;
				cout << "ref_read         : " << ref_str << "  coordinate: "<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
				cout << "  result: correct " << endl;
			} else {
				it_result->correct = false;
				cout << "ref_read         : " << ref_str << "  coordinate: "<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
				cout << "  result: not correct" <<endl;
			}
		}
		cout << "--------" << endl;
		for(list<match_result>::iterator it_result = cheap_filter_result.begin(); it_result != cheap_filter_result.end(); ++it_result) {
			string ref_str(fragment_length_, 'A'); 
			ref_str =  getRefSeq((*it_result).coordinate, fragment_length_, ref2_file_name);
			ED_result edit_result = editDistanceCal(ref_str, testee);
			if (edit_result.correct) {
				it_result->correct = true;
				cout << "ref_read         : " << ref_str << "  coordinate: "<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
				cout << "  result: correct " << endl;
			} else {
				it_result->correct = false;
				cout << "ref_read         : " << ref_str << "  coordinate: "<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
				cout << "  result: not correct" <<endl;
			}
		}

		
		for(list<match_result>::iterator it_result = first_filter_result.begin(); it_result != first_filter_result.end(); ++it_result) {
			if (it_result->correct) {
				bool find_cooresponding = false;
				for (list<match_result>::iterator cmp_iter = cheap_filter_result.begin(); cmp_iter != cheap_filter_result.end(); cmp_iter++) {
					if (cmp_iter->correct && ( cmp_iter->coordinate <= it_result->coordinate + 5 ||cmp_iter->coordinate >= it_result->coordinate - 5) )
						find_cooresponding = true;
				}
				if (!find_cooresponding) {
					cout << "ERROR!!! Didn't find cooresponding result for: " << it_result->coordinate << " in cheap_result" << endl;
					exit(1);
				}

			}
		}


	} while (ref_file.good());

	cout << "ALL PASSED!!" << endl;
	ref_file.close();
	store_file.close();
}
