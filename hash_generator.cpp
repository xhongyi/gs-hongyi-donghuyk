/*
 * hash_generator.c
 *
 *  Created on: Oct 5, 2011
 *      Author: mac
 */
//#include "hash_generator.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <assert.h>

//Hash table stored here
deque<deque<deque<int> > > hash_table;

//Hash table size counter
deque<int> hash_table_counter;

int hashVal(string key) {
	int bp_val = 0;
	int hash_val = 0;

	assert(key.length() == KEY_LENGTH);

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

		hash_val = (hash_val << 2) | bp_val;
	}

	return hash_val;
}

int hashIdx(string key) {
	int bp_val = 0;
	int hash_idx = 0;

//	cout << "Key: " << key << " || Length: "<< key.length() << endl;
	assert(key.length() == KEY_LENGTH);

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
		cout << "bp_val" << bp_val << endl;

		hash_idx = (hash_idx << 2) | bp_val;
	}

	return hash_idx;
}

void hashTableGenerator(string ref_name) {
	//Open and read the refernce file
	ifstream ref_file;
	char * file_name;
	file_name = new char (ref_name.size() + 1);
	strcpy(file_name, ref_name.c_str() );

	ref_file.open(file_name);
	if (!ref_file.good() ){
		cerr << "reference file cannot open." << endl;
		exit(1);
	}

	delete file_name;

	cout << "Hello0" << endl;
	//Initialize hash_table.
	hash_table.resize(HASH_FILE_NUM);
	for (int i = 0; i < HASH_FILE_NUM; i++) {
		hash_table[i].resize(1 << (2 * (KEY_LENGTH - HASH_FILE_POW)));
		hash_table_counter[i] = 0;
		cout << "i: " << i << endl;
	}

	cout << "Hello1" << endl;

	//Read in the first key
	char temp_key[KEY_LENGTH + 1];
	char temp_char;
	string keystr(KEY_LENGTH, 'A');
	cout << "keystr: " << keystr << endl;

	//Read the first 12 characters
	ref_file.read(temp_key, KEY_LENGTH);

	//Check if ref file is too short
	if (ref_file.eof()) {
		cerr << "file too short";
		exit(1);
	}
	temp_key[12] = '\0';
	keystr = temp_key;
	cout << "temp_key: " << temp_key << endl;
	cout << "keystr: " << keystr << endl;

	deque<int>::iterator iter;
	int counter = 0;

	//Read the ref file until the end of file.
	do {
		//Compute idx and hash_value
		int idx = hashIdx(keystr);
		int hash_value = hashVal(keystr);
		cout << "idx: " << idx << endl;
		cout << "hash_value: " << hash_value << endl;

		//Add to hash_table
		cout << "counter: " << counter << endl;
		hash_table[idx][hash_value].push_front(counter);
		hash_table_counter[idx]++;
/*
		//Add to hash_table list
		if (hash_table[idx][hash_value].size() == 0) {
			cout << "first saw" << endl;
			hash_table[idx][hash_value].push_back(counter);
		}
		else {
			iter = hash_table[idx][hash_value].begin();
			while (iter != hash_table[idx][hash_value].end() && counter > *iter) {
				cout << "*iter: " << *iter << endl;
				iter++;
			}
			hash_table[idx][hash_value].insert(iter, counter);
		}
*/

		//Read next character
		ref_file >> temp_char;
		keystr = keystr.substr(1, KEY_LENGTH - 1) + temp_char;
		cout << "keystr: " << keystr << endl;

		//Increment counter
		counter++;
	} while (ref_file.good() );

	ref_file.close();
}

void hashFileWriter(string hash_name) {
	char* store_file_name = new char [hash_name.size() + 1 + HASH_FILE_POW];
	ofstream store_file;
	for (int i = 0; i < HASH_FILE_NUM; i++) {
		sprintf(store_file_name, "%s_%d", hash_name.c_str(), i);
		store_file_name[hash_name.size() + 3 + i/10] = '\0';
		store_file.open(store_file_name);
		store_file << hash_table_counter[i] + 1 << (2 * (KEY_LENGTH - HASH_FILE_POW) ) << endl;
		for (int j = 0; j < 1 << (2 * (KEY_LENGTH - HASH_FILE_POW) ); j++) {
			int entry_size = hash_table[i][j].size();
			store_file << entry_size << endl;
			for (int k = 0; k < entry_size; k++)
				store_file << hash_table[i][j][k] << " ";
			if (entry_size != 0)
				store_file << endl;
		}
		store_file.close();
	}
}

