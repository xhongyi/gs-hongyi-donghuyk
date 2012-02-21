/*
 * bit_mask.cpp
 *
 *  Created on: Feb 13, 2012
 *      Author: hxin
 */

#include "common.h"
#include "bit_mask.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

bitmask::bitmask() {
	for (int i = 0; i < 7; i++) {
		success[i] = 0;
		fail[i] = 0;
	}
}

bitmask::~bitmask() {
}

void bitmask::load_mask(int hash_table_num) {

	for (int i = 0; i < 7; i++) {
		success[i] = 0;
		fail[i] = 0;
	}

	char filename[30];
	ifstream data_file;
	int bm_size;
	int raw_data;

	sprintf(filename, "entry_bm_%d", hash_table_num);
	data_file.open(filename);
	for (int i = 0; i < INDEX_NUM; i++) {
		data_file >> raw_data;
		entry_bm[i].level = (bm_level) raw_data;
		data_file >> entry_bm[i].index;
	}
	data_file.close();

	//skip the first 2 levels of bm
	/*
	sprintf(filename, "first_bm_%d", hash_table_num);
	data_file.open(filename);
	data_file >> bm_size;
	first_bm = new first_mask[bm_size];
	for (int i = 0; i < bm_size; i++) {
		for (int j = 0; j < 16; j++) {
			data_file >> raw_data;
			first_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	data_file.close();

	sprintf(filename, "second_bm_%d", hash_table_num);
	data_file.open(filename);
	data_file >> bm_size;
	second_bm = new second_mask[bm_size];
	for (int i = 0; i < bm_size; i++) {
		for (int j = 0; j < 64; j++) {
			data_file >> raw_data;
			second_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	data_file.close();
	*/

	sprintf(filename, "third_bm_%d", hash_table_num);
	data_file.open(filename);
	data_file >> bm_size;
	third_bm = new third_mask[bm_size];
	for (int i = 0; i < bm_size; i++) {
		for (int j = 0; j < 2048; j++) {
			data_file >> raw_data;
			third_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	data_file.close();

	sprintf(filename, "forth_bm_%d", hash_table_num);
	data_file.open(filename);
	data_file >> bm_size;
	forth_bm = new forth_mask[bm_size];
	for (int i = 0; i < bm_size; i++) {
		for (int j = 0; j < 16384; j++) {
			data_file >> raw_data;
			forth_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	data_file.close();

	sprintf(filename, "fifth_bm_%d", hash_table_num);
	data_file.open(filename);
	data_file >> bm_size;
	fifth_bm = new fifth_mask[bm_size];
	for (int i = 0; i < bm_size; i++) {
		for (int j = 0; j < 131072; j++) {
			data_file >> raw_data;
			fifth_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	data_file.close();

	sprintf(filename, "sixth_bm_%d", hash_table_num);
	data_file.open(filename);
	data_file >> bm_size;
	sixth_bm = new sixth_mask[bm_size];
	for (int i = 0; i < bm_size; i++) {
		for (int j = 0; j < 1048576; j++) {
			data_file >> raw_data;
			sixth_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	data_file.close();
}

void bitmask::free_mask() {
	//delete[] first_bm;
	//delete[] second_bm;
	delete[] third_bm;
	delete[] forth_bm;
	delete[] fifth_bm;
	delete[] sixth_bm;
}

bool bitmask::test_mask(int target_coor, int hash_val) {
	if (entry_bm[hash_val].level == ZERO)
		return false;
	else {
		bool lower_cover = false;
		bool upper_cover = false;
		int lower_bound = target_coor - max_indel_num;
		int upper_bound = target_coor + max_indel_num;
		if (lower_bound < 0)
			lower_bound = 0;

		int index = entry_bm[hash_val].index;

		switch (entry_bm[hash_val].level) {
		case FIRST:
			lower_cover = (1 << ((lower_bound >> 1) % 8))
					&& first_bm[index].mask[((lower_bound >> 1) % 128) / 8];
			upper_cover = (1 << ((upper_bound >> 1) % 8))
					&& first_bm[index].mask[((upper_bound >> 1) % 128) / 8];
			break;
		case SECOND:
			lower_cover = (1 << ((lower_bound >> 1) % 8))
					&& second_bm[index].mask[((lower_bound >> 1) % 512) / 8];
			upper_cover = (1 << ((upper_bound >> 1) % 8))
					&& second_bm[index].mask[((upper_bound >> 1) % 512) / 8];
			break;
		case THIRD:
			lower_cover = (1 << ((lower_bound >> 1) % 8))
					&& third_bm[index].mask[((lower_bound >> 1) % 16384) / 8];
			upper_cover = (1 << ((upper_bound >> 1) % 8))
					&& third_bm[index].mask[((upper_bound >> 1) % 16384) / 8];
			break;
		case FORTH:
			lower_cover = (1 << ((lower_bound >> 1) % 8))
					&& forth_bm[index].mask[((lower_bound >> 1) % 131072) / 8];
			upper_cover = (1 << ((upper_bound >> 1) % 8))
					&& forth_bm[index].mask[((upper_bound >> 1) % 131072) / 8];
			break;
		case FIFTH:
			lower_cover = (1 << ((lower_bound >> 1) % 8))
					&& fifth_bm[index].mask[((lower_bound >> 1) % 1048576) / 8];
			upper_cover = (1 << ((upper_bound >> 1) % 8))
					&& fifth_bm[index].mask[((upper_bound >> 1) % 1048576) / 8];
			break;
		case SIXTH:
			lower_cover = (1 << ((lower_bound >> 1) % 8))
					&& sixth_bm[index].mask[((lower_bound >> 1) % 8388608) / 8];
			upper_cover = (1 << ((upper_bound >> 1) % 8))
					&& sixth_bm[index].mask[((upper_bound >> 1) % 8388608) / 8];
			break;
		}
		if (lower_cover || upper_cover) {
			return true;
		} else {
			success[entry_bm[hash_val].level - 1]++;
			return false;
		}
	}
}

long long int bitmask::get_eval_data(bool is_success, int idx) {
	if (is_success)
		return success[idx];
	else
		return fail[idx];
}

bool bitmask::over_fifth(int hash_val) {
	if (entry_bm[hash_val].level >= 5)
		return true;
	else
		return false;
}

int bitmask::get_level(int hash_val) {
	return (int) entry_bm[hash_val].level;
}

void bitmask::record_fail(int hash_val) {
	fail[entry_bm[hash_val].level - 1]++;
}
