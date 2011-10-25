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
	deque<int>::iterator iter;
	int coor_counter = -12;
	int flag_ignore = 0;
	int monitor_counter  = 0;	// For operation monitoring
	int monitor_counter2 = 0;	// For operation monitoring

	ref_file.open(ref_name.c_str());
	if (!ref_file.good()) {
		cerr << "reference file cannot open." << endl;
		exit(1);
	}

	//cout << "Hello0" << endl;

	//Initialize hash_table.
	//	hash_table.resize(1 << (2 * KEY_LENGTH) );
	//	hash_table_counter = 0;

	//cout << "Hello1" << endl;

	//Read in the first key
	char temp_key[KEY_LENGTH + 1];
	char temp_char;
	string keystr(KEY_LENGTH, 'A');
	//cout << "keystr: " << keystr << endl;

	//throw out the first several Ns
	do {
		ref_file >> temp_char;
		coor_counter++;
		//cout << "coor_counter: " << coor_counter << endl;
	} while (temp_char == 'N');

	//update the first non N character
	temp_key[0] = temp_char;
	//cout << "temp_char: " << temp_char << endl;

	//Read the first 12 characters
	for (int i = 1; i < 12; i++) {
		//cout << "i: " << i << endl;
		ref_file >> temp_char;
		coor_counter++;
		//cout << "coor_counter: " << coor_counter << endl;
		temp_key[i] = temp_char;

		//Check if the file is too short
		if (ref_file.eof()) {
			cerr << "file too short";
			exit(1);
		}
		//Check if next N come too close
		if (temp_char == 'N') {
			cerr << "Too short useful String A" << endl;
			exit(1);
		}
	}

	temp_key[12] = '\0';
	keystr = temp_key;
	//cout << "temp_key: " << temp_key << endl;
	//cout << "keystr: " << keystr << endl;

	//Read the ref file until the end of file.
	do {
		//Compute idx and hash_value
		if (flag_ignore == 0) {
			int hash_value = hashVal(keystr);
		//cout << "hash_value: " << hash_value << endl;

		//Add to hash_table
		//cout << "coor_counter: " << coor_counter << endl;
			hash_table[hash_value].push_front(coor_counter);
			hash_table_counter++;
		} else {
			flag_ignore = 0;
		        //	cout << "check ignore: "; 
			for (int i = 0; i < 12; i++ ) {
		        //	cout << keystr[i];
				if (keystr[i] == 'N') {
					flag_ignore = 1;
				}	
			}
		        //cout << endl;
		}
		/*
		 //Add to hash_table list
		 if (hash_table[idx][hash_value].size() == 0) {
		 //cout << "first saw" << endl;
		 hash_table[idx][hash_value].push_back(counter);
		 }
		 else {
		 iter = hash_table[idx][hash_value].begin();
		 while (iter != hash_table[idx][hash_value].end() && counter > *iter) {
		 //cout << "*iter: " << *iter << endl;
		 iter++;
		 }
		 hash_table[idx][hash_value].insert(iter, counter);
		 }
		 */

		//Read next character
		ref_file >> temp_char;
		//Increment counter
		coor_counter++;

		bool fresh_start = false; //indicater for a new start after large area of N
		//If enters N area, throw them away
		while (temp_char == 'N') {
			if (ref_file.eof() ) {
				ref_file.close();
				return;
			}
			fresh_start = true;
			ref_file >> temp_char;
			coor_counter++;
			//cout << "coor_counter: " << coor_counter << endl;
		}
		if (fresh_start) {
			//cout << "In a fresh start!" << endl;
			keystr[0] = temp_char;
			for (int i = 1; i < 12; i++) {
				//cout << 'i' << i << endl;
				ref_file >> temp_char;
				coor_counter++;
				//cout << "coor_counter: " << coor_counter << endl;
				//Check if next N come too close
				if (temp_char == 'N') {
					cerr << "Too short useful String B" << endl;
					flag_ignore = 1;
					//exit(1);
				}
				keystr[i] = temp_char;
			}
		} else
			keystr = keystr.substr(1, KEY_LENGTH - 1) + temp_char;
		//cout << "keystr: " << keystr << endl;
		// add process monitoring loop
		monitor_counter  = monitor_counter  +1;		// For operation monitoring
		monitor_counter2 = monitor_counter2 +1;		
		if (monitor_counter >= 1000000) {
		        cout << "hash generator: " << monitor_counter2 << endl;
		        monitor_counter = 0;
		}
	} while (ref_file.good());

	ref_file.close();
}

void hashFileWriter(string hash_name) {
	int monitor_counter = 0;	// for operation monitoring
	int monitor_counter2 = 0;	// for operation monitoring
	ofstream store_file;
	store_file.open(hash_name.c_str());
	//store_file << "**Debug: hash_table_counter: " << hash_table_counter[i] << endl;
	//store_file << "**Debug: 1 << (2 * (KEY_LENGTH - HASH_FILE_POW) ): " << (1 << (2 * (KEY_LENGTH - HASH_FILE_POW) ) ) << endl;
	store_file << hash_table_counter + INDEX_NUM << endl;
	for (int j = 0; j < INDEX_NUM; j++) {
		int entry_size = hash_table[j].size();
		store_file << entry_size << endl;
		while (!hash_table[j].empty()) {
			store_file << hash_table[j].back() << " ";
			hash_table[j].pop_back();
		}
		if (entry_size != 0)
			store_file << endl;
		monitor_counter  = monitor_counter  +1;		// For operation monitoring
		monitor_counter2 = monitor_counter2 +1;
		if (monitor_counter >= 1000000) {
		        cout << "hash writer: " << monitor_counter2 << endl;
		        monitor_counter = 0;
		}
	}
	store_file.close();
}

