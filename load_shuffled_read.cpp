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

void loadTestRead(string test_file_name) {
	srand(time(NULL) );

	ifstream input_file;
	input_file.open(test_file_name.c_str());
	int size = 0;
	char test_char;
	string temp(fragment_length_, 'A');
	//fragment set before shuffle.
	deque<string> unshuffled_set;

	//Read out the first fragment
	do {
		input_file >> test_char;
	}
	while (test_char == 'N');

	temp[0] = test_char;
	for (int i = 1; i < fragment_length_; i++) {
		input_file >> test_char;
		if (test_char == 'N') {
			i = 0;
			input_file >> test_char;
		} else
			temp[i] = test_char;
	}
	size = 1;
	unshuffled_set.push_back(temp);

	do {
		if (size == READ_SIZE)
			break;
		input_file >> test_char;
		if (test_char == 'N') {
			for (int i = 0; i < fragment_length_; i++) {
				input_file >> test_char;
				if (test_char == 'N') {
					i = 0;
					input_file >> test_char;
				}
				if (test_char != 'N') {
					temp[i] = test_char;
				}
				if (!input_file.good())
					break;
			}
		} else {
			temp = temp.substr(1, fragment_length_ - 1) + test_char;
		}
		size++;
		unshuffled_set.push_back(temp);
	}
	while (input_file.good());

	deque<string>::iterator unshuffled_iter;
	for (int i = 0; i < size; i++) {
		unshuffled_iter = unshuffled_set.begin();
		int increment = (int)(unshuffled_set.size() * ( (rand() % size) / (double) size) );
		unshuffled_iter = unshuffled_iter + increment;
		cout << *unshuffled_iter << endl;
		unshuffled_set.erase(unshuffled_iter);
	}

	/*
	1if (unshuffled_set.empty() )
		cout << "empty correct!" << endl;
	else
		cout << "Not empty!! Incorrect!" << endl;
	*/

}
