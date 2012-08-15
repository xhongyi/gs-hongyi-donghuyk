/*
 * load_shuffled_read.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: mac
 */

#include "load_shuffled_read.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <deque>

#define SINGLE_CHROMO_SIZE 50000
#define RAW_READ_LENGTH 730
#define TRUE_READ_LENGTH 720

char randomBP() {
	int mismatch_case = rand() % 4;
	switch (mismatch_case) {
	case 1:
		return 'A';
	case 2:
		return 'C';
	case 3:
		return 'G';
	case 4:
		return 'T';
	default:
		return 'A';
	}
}

void add_error(string& temp, int error_num) {
	for (int i = 0; i < error_num; i++) {
		int location = rand() % TRUE_READ_LENGTH;
		int error_type = rand() % 4;
		switch (error_type) {
		//Do nothing
		case 0:
			break;
		//Mutation
		case 1:
			temp[location] = randomBP();
			break;
		//Insertion
		case 2:
			temp = temp.substr(0, location) + randomBP() + temp.substr(location, RAW_READ_LENGTH - 1 - location);
			break;
		//Deletion
		case 3:
			temp = temp.substr(0, location) + temp.substr(location + 1, RAW_READ_LENGTH - 1 - location);
			break;
		default:
			break;
		}
	}
}

void loadTestRead(string test_file_name, int error_num) {
	ifstream input_file;
	srand(time(NULL));
	input_file.open(test_file_name.c_str());
	int size = 0;
	char test_char;
	DNA_info temp_info;
	string temp(RAW_READ_LENGTH, 'A');
	string null_string;
	//fragment set before shuffle.
	deque<DNA_info> unshuffled_set;

	for (int chromo_count = 0; chromo_count < 20; chromo_count++) {
		int location = 0;
		int inner_size = 0;

		cout << "chromo_count: " << chromo_count << endl;

		//Take out the chromosome name
		getline(input_file, null_string);
		cout << null_string << endl;

		//Read out the first fragment
		do {
			location++;
			input_file >> test_char;
		} while (test_char == 'N');

		do {
			for (int i = 0; i < RAW_READ_LENGTH; i++) {
				if (test_char == 'N') {
					i = -1;
				} else if (test_char == '>') {
					break;
				} else {
					temp[i] = test_char;
				}
				location++;
				input_file >> test_char;
			}

			//If happens to read to the next chromo, break here.
			if (test_char == '>')
				break;

			add_error(temp, error_num);

			temp_info.chromo_num = chromo_count + 1;
			temp_info.location = location - RAW_READ_LENGTH;
			temp_info.seq_72 = temp.substr(0, 72);
			temp_info.seq_108 = temp.substr(0, 108);
			temp_info.seq_180 = temp.substr(0, 180);
			temp_info.seq_360 = temp.substr(0, 360);
			temp_info.seq_720 = temp;

			unshuffled_set.push_back(temp_info);
			inner_size++;

			//Read out random number of blank bp for next read.
			int skip = rand() % 100;
			for (int i = 0; i < skip; i++) {
				location++;
				input_file >> test_char;
			}

			cout << "inner_size: " << inner_size << endl;
		} while (inner_size < SINGLE_CHROMO_SIZE);

		//Finish the last chromosome
		do {
			input_file >> test_char;
		} while (test_char != '>');
	}

	//Output_files
	ofstream out_72;
	ofstream out_108;
	ofstream out_180;
	ofstream out_360;
	ofstream out_720;
	ofstream out_loc;

	out_72.open("read_72");
	out_108.open("read_108");
	out_180.open("read_180");
	out_360.open("read_360");
	out_720.open("read_720");
	out_loc.open("correct_location");

	//Dump reads while it's not empty
	deque<DNA_info>::iterator unshuffled_iter;
	int out_count = 1;
	while (!unshuffled_set.empty()) {
		unshuffled_iter = unshuffled_set.begin();
		/*
		 int increment = (int) (unshuffled_set.size()
		 ((rand() % unshuffled_set.size() ) / (double) unshuffled_set.size() ));
		 unshuffled_iter = unshuffled_iter + increment;
		 */

		//Dump name
		out_72 << "@synthetic_read_" << out_count << endl;
		out_108 << "@synthetic_read_" << out_count << endl;
		out_180 << "@synthetic_read_" << out_count << endl;
		out_360 << "@synthetic_read_" << out_count << endl;
		out_720 << "@synthetic_read_" << out_count << endl;

		//Dump sequence
		out_72 << unshuffled_iter->seq_72 << endl;
		out_108 << unshuffled_iter->seq_108 << endl;
		out_180 << unshuffled_iter->seq_180 << endl;
		out_360 << unshuffled_iter->seq_360 << endl;
		out_720 << unshuffled_iter->seq_720 << endl;

		//Dump +
		out_72 << "+" << endl;
		out_108 << "+" << endl;
		out_180 << "+" << endl;
		out_360 << "+" << endl;
		out_720 << "+" << endl;

		//Dump quality
		out_72 << string(72, 'B') << endl;
		out_108 << string(108, 'B') << endl;
		out_180 << string(180, 'B') << endl;
		out_360 << string(360, 'B') << endl;
		out_720 << string(720, 'B') << endl;

		//Dump chromo and loc
		out_loc << unshuffled_iter->chromo_num << " "
				<< unshuffled_iter->location << " " << unshuffled_iter->original_seq << endl;

		//erase dumped read
		unshuffled_set.erase(unshuffled_iter);
		out_count++;
	}
	/*
	 1if (unshuffled_set.empty() )
	 cout << "empty correct!" << endl;
	 else
	 cout << "Not empty!! Incorrect!" << endl;
	 */

}
