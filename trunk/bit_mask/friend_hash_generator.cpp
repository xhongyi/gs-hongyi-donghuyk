/*
 * friend_hash_generator.cpp
 *
 *  Created on: Feb 23, 20KEY_LENGTH
 *      Author: hxin
 */
#include "friend_hash_generator.h"
#include "string"
#include <cstdio>

bool friend_hash[INDEX_NUM][MAX_SEGMENT_NUM][MAX_INDEL_NUM + 1];

void initializeFriendHash() {
	for (int i = 0; i < INDEX_NUM; i++) {
		for (int j = 0; j < MAX_SEGMENT_NUM; j++) {
			for (int k = 0; k < MAX_INDEL_NUM + 1; k++) {
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
	for (int i = 1; i <= MAX_SEGMENT_NUM; i++) {
		//Iterate In/Dels
		for (int j = 0; j <= MAX_INDEL_NUM; j++) {
			second_half = fragment.substr(i * KEY_LENGTH - j, KEY_LENGTH);
			if (second_half.find('N') == string::npos) {

				index_string = first_half.substr(0, KEY_LENGTH / 2)
						+ second_half.substr(0, KEY_LENGTH / 2);
				index = hashVal(index_string);
				for (int k = j; k <= MAX_INDEL_NUM; k++) {
					friend_hash[index][i][k] = true;
				}
			}

			second_half = fragment.substr(i * KEY_LENGTH + j, KEY_LENGTH);
			if (second_half.find('N') == string::npos) {

				index_string = first_half.substr(0, KEY_LENGTH / 2)
						+ second_half.substr(0, KEY_LENGTH / 2);
				index = hashVal(index_string);
				for (int k = j; k <= MAX_INDEL_NUM; k++) {
					friend_hash[index][i][k] = true;
				}
			}
		}
	}
}

void analyzeContig(string ref_name) {
	ifstream ref_file;
	ref_file.open(ref_name.c_str());
	if (!ref_file.good()) {
		cerr << "reference file cannot open" << endl;
		exit(1);
	}

	string temp_fragment(KEY_LENGTH * (MAX_SEGMENT_NUM + 1) + MAX_INDEL_NUM,
			'A');
	char temp_char;

	for (int i = 0; i < KEY_LENGTH * (MAX_SEGMENT_NUM + 1); i++) {
		ref_file >> temp_fragment[i];
	}

	analyzeFragment(temp_fragment);

	while (ref_file.good()) {
		ref_file >> temp_char;
		temp_fragment = temp_fragment.substr(1,
				KEY_LENGTH * (MAX_SEGMENT_NUM + 1) + MAX_INDEL_NUM - 1)
				+ temp_char;
		analyzeFragment(temp_fragment);
	}

	for (int i = 0; i < KEY_LENGTH * (MAX_SEGMENT_NUM + 1) + MAX_INDEL_NUM;
			i++) {
		temp_fragment = temp_fragment.substr(1,
				KEY_LENGTH * (MAX_SEGMENT_NUM + 1) + MAX_INDEL_NUM - 1)
				+ temp_char;
		analyzeFragment(temp_fragment);
	}
	ref_file.close();
}

void writeFriendHash(char store_file_name[]) {
	ofstream friend_file;
	for (int i = 0; i <= MAX_INDEL_NUM; i++) {
		char temp_file_name[50];
		sprintf(temp_file_name, "%s_%i", store_file_name, i);
		friend_file.open(temp_file_name);
		unsigned char mask[2];
		for (int j = 0; j < INDEX_NUM; j++) {
			for (int k = 0; k < MAX_SEGMENT_NUM; k++) {

			}
		}
	}
}
