/*
 * hash_generator.c
 *
 *  Created on: Oct 5, 2011
 *      Author: mac
 */
#include"hash_generator.h"
#include <iostream>
#include <fstream>

int hashVal(string key) {
	int bp_val = 0;
	int hash_val = 0;

	assert (key.length() == KEY_LENGTH);

	for (int i = HASH_FILE_POW; i < KEY_LENGTH; i++) {
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
		default:
			cerr << "Wrong bp: " << key[i];
			exit(1);
		}

		hash_val = (bp_val << 2) | hash_val;
		i++;
	}

	return hash_val;
}

int hashIdx(string key) {
	int bp_val = 0;
	int hash_idx = 0;

	assert (key.length() == KEY_LENGTH);

	for (int i = 0; i < HASH_FILE_POW; i++) {
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
		default:
			cerr << "Wrong bp: " << key[i];
			exit(1);
		}

		hash_val = (bp_val << 2) | hash_val;
		i++;
	}

	return hash_val;
}

void hashTableGenerator(string ref_name) {
	//Open and read the refernce file
	ifstream ref_file;

	if ( !(ref_file.open(ref_name) ) ) {
		cerr << "reference file cannot open." << endl;
		exit(1);
	}

	//Initialize hash_table.
	hash_table.resize(HASH_FILE_NUM);
	for (int i = 0; i < HASH_FILE_NUM; i++) {
		hash_table[i].resize(1 << (2 * (KEY_LENGTH - HASH_FILE_POW) ) );
	}

	//Read in the first key
	char tempkey[KEY_LENGTH];
	char temp_char;
	string keystr('A', KEY_LENGTH);

	for (int i = 0; i < KEY_LENGTH; i++) {
		ref_file.read(tempkey, KEY_LENGTH);

		//Check if ref file is too short
		if (ref_file.eof() ) {
			cerr << "file too short";
			exit(1);
		}
		keystr.copy(tempkey, KEY_LENGTH);
	}

	//Read the ref file until the end of file.
	while (ref_file.eof() ) {

	}
}
