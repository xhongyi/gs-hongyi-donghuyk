/*
 * common.c
 *
 *  Created on: Oct 7, 2011
 *      Author: mac
 */
#include <assert.h>
#include "common.h"
#include <iostream>
#include <cstdlib>

int max_indel_num = 0;
int max_diff_num = 0;
bool select_cheapest = true;

__device__ int hashVal(string key) {
	int bp_val = 0;
	int hash_val = 0;

	assert(key.length() == KEY_LENGTH);

	for (int i = 0; i < KEY_LENGTH; i++) {
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


void set_max_indel_num(int num) {
	max_indel_num = num;
}

void set_max_diff_num(int num) {
	if (num > MAX_ERROR_NUM) {
		cerr << "Does not support error more than 10. Please contact author gohongyi@gmail.com for more information" << endl;
	}
	max_diff_num = num;
}

void set_select_cheapest(bool choose) {
	select_cheapest = choose;
}

/*
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
		//cout << "bp_val" << bp_val << endl;		

		hash_idx = (hash_idx << 2) | bp_val;
	}

	return hash_idx;
}
*/
