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
int fragment_length_ = 108;
int key_number_ = 9;
bool select_cheapest = true;
bool reverse_mode = false;
bool print_detail = true;
bool bit_mask_on = false;

int hashVal(string key) {
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
		case 'N':
			return -1;
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

void set_bit_mask(bool choice) {
	bit_mask_on = choice;
}

void set_fragment_length(int num) {
	fragment_length_ = num;
}

void set_key_number(int num) {
	key_number_ = num;
}
char reverseCompleteChar(char c) {
	char ret;
	switch (c) {
		case 'A':
					ret = 'T';
					break;
		case 'T':
					ret = 'A';
					break;
		case 'C':
					ret = 'G';
					break;
		case 'G':
					ret = 'C';
					break;
		default:
					ret = 'N';
					break;
	}
	return ret;
}

void reverseComplete (const char *seq, char *rcSeq , int length) {
	for (int i=0; i<length; i++) {
		rcSeq[i]=reverseCompleteChar (seq[length-1-i]) ;
	}
}

void set_reverse_mode(bool set_reverse_mode) {
	reverse_mode = set_reverse_mode;
}

void set_print_detail(bool set_print_detail) {
	print_detail = set_print_detail;
}


