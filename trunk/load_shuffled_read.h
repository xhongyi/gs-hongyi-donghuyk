/*
 * load_shuffled_read.h
 *
 *  Created on: Nov 24, 2011
 *      Author: mac
 */

#ifndef LOAD_SHUFFLED_READ_H_
#define LOAD_SHUFFLED_READ_H_

#include <string>
#include <fstream>
#include "common.h"

struct DNA_info {
	string seq_72;
	string seq_108;
	string seq_180;
	string seq_360;
	string seq_720;
	string original_seq;
	int chromo_num;
	int location;
};

using namespace std;

void loadTestRead(string test_file_name, int error_num);

#endif /* LOAD_SHUFFLED_READ_H_ */
