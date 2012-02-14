/*
 * bit_mask_generator.cpp
 *
 *  Created on: Jan 15, 2012
 *      Author: mac
 */
#include "bit_mask_generator.h"
#include "bit_mask.h"
#include <queue>
#include <string>
#include <cstdio>
#include <cstdlib>

int bot_range;

queue<first_mask> first_bm;
queue<second_mask> second_bm;
queue<third_mask> third_bm;
queue<forth_mask> forth_bm;
queue<fifth_mask> fifth_bm;
queue<sixth_mask> sixth_bm;
queue<seventh_mask> seventh_bm;

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

	int coor_num;
	int coordinate;
	int bm_idx;

	// Read index number & coordinate based on index number
	for (int i = 0; i < INDEX_NUM; i++) {
		hash_file >> coor_num;

		//Determine the bm level
		int temp_coor_num = coor_num;
		entry_bm[i].level = ZERO;
		while (temp_coor_num > 0) {
			entry_bm[i].level = (bm_level) ((int) entry_bm[i].level + 1);
			temp_coor_num /= 10;
			if ((int) entry_bm[i].level > 7) {
				cout << "Opps here there is an outlier with coor_num: "
						<< coor_num << endl;
				cout << "i = " << i << endl;
				exit(1);
			}
		}

		// Read coordinate
		switch (entry_bm[i].level) {
		case ZERO:
			break;
		case FIRST:
			first_mask mask1;
			mask1.mask = 0;
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
		case SIXTH:
			sixth_mask mask6;
			for (int j = 0; j < 131072; j++)
				mask6.mask[j] = 0;
			for (int j = 0; j < coor_num; j++) {
				hash_file >> coordinate;
				bm_idx = ((coordinate >> 1) % 1048576) / 8;
				mask6.mask[bm_idx] |= 1 << ((coordinate >> 1) % 8);
			}
			entry_bm[i].index = sixth_bm.size();
			sixth_bm.push(mask6);
			break;
		case SEVENTH:
			seventh_mask mask7;
			for (int j = 0; j < 1048576; j++)
				mask7.mask[j] = 0;
			for (int j = 0; j < coor_num; j++) {
				hash_file >> coordinate;
				bm_idx = ((coordinate >> 1) % 8388608) / 8;
				mask7.mask[bm_idx] |= 1 << ((coordinate >> 1) % 8);
			}
			entry_bm[i].index = seventh_bm.size();
			seventh_bm.push(mask7);
			break;
		}
	}
	hash_file.close();
}

void writeBitMask(int hash_table_num) {
	char filename[30];
	ofstream entry_bm_file;
	sprintf(filename, "entry_bm_%d", hash_table_num);
	entry_bm_file.open(filename);
	for (int i = 0; i < INDEX_NUM; i++) {
		entry_bm_file << entry_bm[i].level << endl;
		entry_bm_file << entry_bm[i].index << endl;
	}

	ofstream bm_1_file;
	sprintf(filename, "first_bm_%d", hash_table_num);
	bm_1_file.open(filename);
	bm_1_file << first_bm.size() << endl;
	while (!first_bm.empty()) {
		bm_1_file << (int) first_bm.front().mask << endl;
		first_bm.pop();
	}

	ofstream bm_2_file;
	sprintf(filename, "second_bm_%d", hash_table_num);
	bm_2_file.open(filename);
	bm_2_file << second_bm.size() << endl;
	while (!second_bm.empty()) {
		second_mask temp = second_bm.front();
		for (int i = 0; i < 16; i++) {
			bm_2_file << (int) temp.mask[i] << " ";
		}
		bm_2_file << endl;
		second_bm.pop();
	}

	ofstream bm_3_file;
	sprintf(filename, "thrid_bm_%d", hash_table_num);
	bm_3_file.open(filename);
	bm_3_file << third_bm.size() << endl;
	while (!third_bm.empty()) {
		third_mask temp = third_bm.front();
		for (int i = 0; i < 128; i++) {
			bm_3_file << (int) temp.mask[i] << " ";
		}
		bm_3_file << endl;
		third_bm.pop();
	}

	ofstream bm_4_file;
	sprintf(filename, "forth_bm_%d", hash_table_num);
	bm_4_file.open(filename);
	bm_4_file << first_bm.size() << endl;
	while (!forth_bm.empty()) {
		forth_mask temp = forth_bm.front();
		for (int i = 0; i < 2048; i++) {
			bm_4_file << (int) temp.mask[i] << " ";
		}
		bm_4_file << endl;
		forth_bm.pop();
	}

	ofstream bm_5_file;
	sprintf(filename, "fifth_bm_%d", hash_table_num);
	bm_5_file.open(filename);
	bm_5_file << fifth_bm.size() << endl;
	while (!fifth_bm.empty()) {
		fifth_mask temp = fifth_bm.front();
		for (int i = 0; i < 16384; i++) {
			bm_5_file << (int) temp.mask[i] << " ";
		}
		bm_5_file << endl;
		fifth_bm.pop();
	}

	ofstream bm_6_file;
	sprintf(filename, "sixth_bm_%d", hash_table_num);
	bm_6_file.open(filename);
	bm_6_file << sixth_bm.size() << endl;
	while (!sixth_bm.empty()) {
		sixth_mask temp = sixth_bm.front();
		for (int i = 0; i < 131072; i++) {
			bm_6_file << (int) temp.mask[i] << " ";
		}
		bm_6_file << endl;
		sixth_bm.pop();
	}

	ofstream bm_7_file;
	sprintf(filename, "seventh_bm_%d", hash_table_num);
	bm_7_file.open(filename);
	bm_7_file << seventh_bm.size() << endl;
	while (!seventh_bm.empty()) {
		seventh_mask temp = seventh_bm.front();
		for (int i = 0; i < 1048576; i++) {
			bm_7_file << (int) temp.mask[i] << " ";
		}
		bm_7_file << endl;
		seventh_bm.pop();
	}

}
