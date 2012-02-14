/*
 * bit_mask_analyzer.cpp
 *
 *  Created on: Jan 27, 2012
 *      Author: hxin
 */

#include "bit_mask_analyzer.h"

int* hash_table;
int* coordinate;
int bot_bit_mask[INDEX_NUM];
int top_bit_mask[INDEX_NUM];

void initialization() {
	maskReset(top_bit_mask);
	maskReset(bot_bit_mask);
}

void unloadBitMask() {
	maskReset(top_bit_mask);
	maskReset(bot_bit_mask);
	free(hash_table);
	free(coordinate);
}

void loadBitMask(string hash_table_name, string top_bm_name,
		string bot_bm_name) {
	int temp;
	hashReconstructor(&hash_table, &coordinate, hash_table_name.c_str());
	cout << "aaa" << endl;
	maskLoader(top_bit_mask, temp, top_bm_name);
	cout << "bbb" << endl;
	maskLoader(bot_bit_mask, temp, bot_bm_name);
	cout << "ccc" << endl;
}

void evaluateBitMask(int upper_threshold, int lower_threshold,
		ofstream &outfile) {
	for (int i = 0; i < INDEX_NUM; i++) {
		evaluateBitMaskSingle(i, upper_threshold, lower_threshold, outfile);
	}
}

void evaluateBitMaskSingle(int hash_val, int upper_threshold,
		int lower_threshold, ofstream& outfile) {
	//Test coordinate size. If the coordinate size is too small, it is not
	//interested
	if (coordinate[hash_table[hash_val]] >= lower_threshold
			&& coordinate[hash_table[hash_val]] <= upper_threshold) {
		outfile << "Hash Val: " << hash_val << endl;
		outfile << "coordinate size: " << coordinate[hash_table[hash_val]]
				<< endl;
		outfile << "top_bit_mask: ";
		for (int i = 31; i >= 0; i--) {
			if ((1 << i) & top_bit_mask[hash_val])
				outfile << 1;
			else
				outfile << 0;
		}
		outfile << endl;
		outfile << "bot_bit_mask: ";
		for (int i = 31; i >= 0; i--) {
			if ((1 << i) & bot_bit_mask[hash_val])
				outfile << 1;
			else
				outfile << 0;
		}
		outfile << endl;
	}
}
