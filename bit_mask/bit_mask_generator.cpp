/*
 * bit_mask_generator.cpp
 *
 *  Created on: Jan 15, 2012
 *      Author: mac
 */
#include "bit_mask_generator.h"

int mask_range;

int bit_mask[INDEX_NUM];

void clearBitMask() {
	for (int i = 0; i < INDEX_NUM; i++)
		bit_mask[i] = 0;
}

void generateBitMask(string hash_table_name) {
	int total_number;
	ifstream hash_file;
	hash_file.open(hash_table_name.c_str() );
	if (!hash_file.good()) {
		cerr << "File " << hash_table_name << " does not exist" << endl;
	}

	hash_file >> total_number;
	//This will return the range of the 1 bit in the bit_mask
	mask_range = total_number / 32 + 1;

	int index;
	int coordinate;

	// Read index number & coordinate based on index number
	for (int i = 0; i < INDEX_NUM; i++) {
		hash_file >> index;
		// Read coordinate
		for (int j = 0; j < index; j++) {
			hash_file >> coordinate;
			bit_mask[i] |= 1 << (coordinate / mask_range);
		}
	}
	hash_file.close();
}

void writeBitMask(string bit_mask_name) {
	ofstream mask_file;
	mask_file.open(bit_mask_name.c_str() );
	mask_file << mask_range << endl;
	for (int i = 0; i < INDEX_NUM; i++)
		mask_file << bit_mask[i] << " ";
}