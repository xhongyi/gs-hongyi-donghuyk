/*
 * friend_hash_generator.cpp
 *
 *  Created on: Feb 23, 20KEY_LENGTH
 *      Author: hxin
 */
#include "friend_hash_generator.h"
#include "string"
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include "common.h"

bool friend_hash[INDEX_NUM][MAX_SEGMENT_NUM_][MAX_INDEL_NUM_ + 1];

int hashVal(string key) {
	int bp_val = 0;
	int hash_val = 0;

	assert(key.length() == KEY_LENGTH);

	for (int i = 0; i < KEY_LENGTH; i++) {
		switch (key[i]) {
		case 'A':
			bp_val = 0;
			break;
		case 'C':
			bp_val = 1;
			break;
		case 'G':
			bp_val = 2;
			break;
		case 'T':
			bp_val = 3;
			break;
		case 'N':
			return -1;
		default:
			cerr << "Wrong bp: " << key[i];
			exit(1);
		}

		hash_val = (hash_val << 2) | bp_val;
	}

	return hash_val;
}

void initializeFriendHash() {
	for (int i = 0; i < INDEX_NUM; i++) {
		for (int j = 0; j < MAX_SEGMENT_NUM_; j++) {
			for (int k = 0; k < MAX_INDEL_NUM_ + 1; k++) {
				friend_hash[i][j][k] = false;
			}
		}
	}
}

void analyzeFragment(string fragment) {
	string first_half = fragment.substr(0, KEY_LENGTH);
	string second_half;
	string index_string;
	int index;

	//If the first segment have has bad bp
	if (first_half.find('N') != string::npos)
		return;

	//Iterate all consecutive segments
	for (int i = 1; i <= MAX_SEGMENT_NUM_; i++) {
		//Iterate In/Dels
		for (int j = 0; j <= MAX_INDEL_NUM_; j++) {
			//For insertions
			second_half = fragment.substr(i * KEY_LENGTH - j, KEY_LENGTH);
			if (second_half.find('N') == string::npos) {

				index_string = first_half.substr(0, KEY_LENGTH / 2)
						+ second_half.substr(0, KEY_LENGTH / 2);
				index = hashVal(index_string);
				for (int k = j; k <= MAX_INDEL_NUM_; k++) {
					friend_hash[index][i][k] = true;
				}
			}

			//For deletions
			second_half = fragment.substr(i * KEY_LENGTH + j, KEY_LENGTH);
			if (second_half.find('N') == string::npos) {

				index_string = first_half.substr(0, KEY_LENGTH / 2)
						+ second_half.substr(0, KEY_LENGTH / 2);
				index = hashVal(index_string);
				for (int k = j; k <= MAX_INDEL_NUM_; k++) {
					friend_hash[index][i][k] = true;
				}
			}
		}
	}
}

void analyzeContig(char ref_name[]) {
	ifstream ref_file;
	ref_file.open(ref_name);
	if (!ref_file.good()) {
		cerr << "reference file cannot open" << endl;
		exit(1);
	}

	string temp_fragment(KEY_LENGTH * (MAX_SEGMENT_NUM_ + 1) + MAX_INDEL_NUM_,
			'A');
	char temp_char;

	for (int i = 0; i < KEY_LENGTH * (MAX_SEGMENT_NUM_ + 1); i++) {
		ref_file >> temp_fragment[i];
	}

	analyzeFragment(temp_fragment);

	int counter = 0;

	while (ref_file.good()) {

		if (counter % 100000 == 0) {
			cout << "counter: " << counter / 100000 << endl;
		}

		ref_file >> temp_char;
		temp_fragment = temp_fragment.substr(1,
				KEY_LENGTH * (MAX_SEGMENT_NUM_ + 1) + MAX_INDEL_NUM_ - 1)
				+ temp_char;
		analyzeFragment(temp_fragment);
		counter++;
	}

	for (int i = 0; i < KEY_LENGTH * (MAX_SEGMENT_NUM_ + 1) + MAX_INDEL_NUM_;
			i++) {
		temp_fragment = temp_fragment.substr(1,
				KEY_LENGTH * (MAX_SEGMENT_NUM_ + 1) + MAX_INDEL_NUM_ - 1)
				+ temp_char;
		analyzeFragment(temp_fragment);
	}
	ref_file.close();
}

void writeFriendHash(char store_file_name[]) {
	ofstream friend_file;
	for (int i = 0; i <= 0/*MAX_INDEL_NUM_*/; i++) {
		char temp_file_name[50];
		sprintf(temp_file_name, "%s_%i", store_file_name, i);
		friend_file.open(temp_file_name);
		unsigned char mask[2];
		for (int j = 0; j < INDEX_NUM; j++) {
			mask[0] = 0;
			mask[1] = 0;
			for (int k = 0; k < MAX_SEGMENT_NUM_; k++) {
				if (friend_hash[j][k][i] == true)
					mask[k / 8] |= 1 << (k % 8);
			}
			//cout << (int) mask[0] << " " << (int) mask[1] << endl;
			friend_file << (int) mask[0] << " " << (int) mask[1] << endl;
		}
		friend_file.close();
	}
}
