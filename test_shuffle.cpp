/*
 * test_shuffle.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: mac
 */

#include "load_shuffled_read.h"
#include <iostream>

using namespace std;

int main() {
	string test_file_name = "result_ref_0";
	string test_input[READ_SIZE];
	int size;
	loadTestRead(test_file_name);
	//cout << "size: " << size << endl;
	for (int i = 0; i < size; i++) {
		//cout << "i: " << i << endl;
		cout << test_input[i] << endl;
	}
	return 0;
}
