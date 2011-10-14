/*
 * hash_generator.c
 *
 *  Created on: Oct 5, 2011
 *      Author: mac
 */
#include "hash_generator.h"
#include <deque>
#include <list>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <assert.h>

list<int> _hash_entry;

//Hash table stored here
deque<list<int> > hash_table(INDEX_NUM, _hash_entry);

//Hash table size counter
int hash_table_counter = 0;

void hashTableGenerator(string ref_name) {
	//Open and read the reference file
	ifstream ref_file;

	ref_file.open(ref_name.c_str() );
	if (!ref_file.good()) {
		cerr << "reference file cannot open." << endl;
		exit(1);
	}

	cout << "Hello0" << endl;

	//Initialize hash_table.
//	hash_table.resize(1 << (2 * KEY_LENGTH) );
//	hash_table_counter = 0;

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
		int hash_value = hashVal(keystr);
		cout << "hash_value: " << hash_value << endl;

		//Add to hash_table
		cout << "counter: " << counter << endl;
		hash_table[hash_value].push_front(counter);
		hash_table_counter++;
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
	} while (ref_file.good());

	ref_file.close();
}

void hashFileWriter(string hash_name) {
	ofstream store_file;
	store_file.open(hash_name.c_str());
	//store_file << "**Debug: hash_table_counter: " << hash_table_counter[i] << endl;
	//store_file << "**Debug: 1 << (2 * (KEY_LENGTH - HASH_FILE_POW) ): " << (1 << (2 * (KEY_LENGTH - HASH_FILE_POW) ) ) << endl;
	store_file << hash_table_counter + INDEX_NUM << endl;
	for (int j = 0; j < INDEX_NUM; j++) {
		int entry_size = hash_table[j].size();
		store_file << entry_size << endl;
		while (!hash_table[j].empty() ) {
			store_file << hash_table[j].back() << " ";
			hash_table[j].pop_back();
		}
		if (entry_size != 0)
			store_file << endl;
	}
	store_file.close();
}

