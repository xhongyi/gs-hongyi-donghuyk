/*
 * bit_mask_generator.cpp
 *
 *  Created on: Jan 15, 2012
 *      Author: mac
 */
#include "bit_mask_generator.h"
#include "bit_mask.h"
#include <queue>

int bot_range;

queue<first_mask> first_bm;
queue<second_mask> second_bm;
queue<third_mask> third_bm;
queue<forth_mask> forth_bm;
queue<fifth_mask> fifth_bm;

bm_info entry_bm[INDEX_NUM];

void clearBitMask() {
	for (int i = 0; i < INDEX_NUM; i++) {
		entry_bm[i].index = 0;
		entry_bm[i].level = FIRST;
	}
	for (int i = 0; i < INDEX_NUM; i++) {
		while (!first_bm.empty())
			first_bm.pop();
		while (!second_bm.empty())
			second_bm.pop();
		while (!third_bm.empty())
			third_bm.pop();
		while (!forth_bm.empty())
			forth_bm.pop();
		while (!fifth_bm.empty())
			fifth_bm.pop();
	}
}

void generateBitMask(string hash_table_name) {
	int total_number;
	ifstream hash_file;
	hash_file.open(hash_table_name.c_str());
	if (!hash_file.good()) {
		cerr << "File " << hash_table_name << " does not exist" << endl;
	}

	hash_file >> total_number;
	//This will return the range of the 1 bit in the bit_mask
	mask_range = total_number / 32 + 1;

	int coor_num;
	int coordinate;
	int bm_idx;

	// Read index number & coordinate based on index number
	for (int i = 0; i < INDEX_NUM; i++) {
		hash_file >> coor_num;

		//Determine the bm level
		int temp_coor_num = coor_num;
		entry_bm[i].level = FIRST;
		temp_coor_num /= 10;
		while (temp_coor_num >= 0) {
			entry_bm[i].level++;
			temp_coor_num /= 10;
		}

		// Read coordinate
		switch (entry_bm[i].level) {
		case FIRST:
			first_mask mask1 = 0;
			for (int j = 0; j < coor_num; j++) {
				hash_file >> coordinate;
				mask1.mask |= 1 << ((coordinate >> 1) % 8);
			}
			entry_bm[i].index = first_bm.size();
			first_bm.push(mask1);
			break;

		case SECOND:
			second_mask mask2;
			for (int j = 0; j < 16; j++)
				mask2.mask[j] = 0;
			for (int j = 0; j < coor_num; j++) {
				hash_file >> coordinate;
				bm_idx = ((coordinate >> 1) % 128) / 8;
				mask2.mask[bm_idx] |= 1 << ((coordinate >> 1) % 8);
			}
			entry_bm[i].index = second_bm.size();
			second_bm.push(mask2);
			break;

		case THIRD:
			third_mask mask3;
			for (int j = 0; j < 128; j++)
				mask3.mask[j] = 0;
			for (int j = 0; j < coor_num; j++) {
				hash_file >> coordinate;
				bm_idx = ((coordinate >> 1) % 1024) / 8;
				mask3.mask[bm_idx] |= 1 << ((coordinate >> 1) % 8);
			}
			entry_bm[i].index = third_bm.size();
			third_bm.push(mask3);
			break;
		case FORTH:
			forth_mask mask4;
			for (int j = 0; j < 2048; j++)
				mask4.mask[j] = 0;
			for (int j = 0; j < coor_num; j++) {
				hash_file >> coordinate;
				bm_idx = ((coordinate >> 1) % 16384) / 8;
				mask4.mask[bm_idx] |= 1 << ((coordinate >> 1) % 8);
			}
			entry_bm[i].index = forth_bm.size();
			forth_bm.push(mask4);
			break;
		case FIFTH:
			fifth_mask mask5;
			for (int j = 0; j < 16384; j++)
				mask5.mask[j] = 0;
			for (int j = 0; j < coor_num; j++) {
				hash_file >> coordinate;
				bm_idx = ((coordinate >> 1) % 131072) / 8;
				mask5.mask[bm_idx] |= 1 << ((coordinate >> 1) % 8);
			}
			entry_bm[i].index = fifth_bm.size();
			fifth_bm.push(mask5);
			break;
		}
	}
	hash_file.close();
}

void writeBitMask(string top_bit_mask_name, string bot_bit_mask_name) {
	ofstream top_mask_file;
	ofstream bot_mask_file;
	top_mask_file.open(top_bit_mask_name.c_str());
	bot_mask_file.open(bot_bit_mask_name.c_str());
	top_mask_file << mask_range << endl;
	bot_mask_file << 0 << endl;
	for (int i = 0; i < INDEX_NUM; i++) {
		top_mask_file << top_bit_mask[i] << " ";
		bot_mask_file << bot_bit_mask[i] << " ";
	}
}
