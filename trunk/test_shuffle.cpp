/*
 * test_shuffle.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: mac
 */

#include "load_shuffled_read.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: ref_gene_name error_num" << endl;
		exit(1);
	}
	
	string test_file_name = argv[1];
	int error = atoi(argv[2]);
	string test_input[READ_SIZE];
	int size;
	loadTestRead(test_file_name, error);
	//cout << "size: " << size << endl;
	for (int i = 0; i < size; i++) {
		//cout << "i: " << i << endl;
		cout << test_input[i] << endl;
	}
	return 0;
}
