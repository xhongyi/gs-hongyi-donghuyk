/*
 * bit_mask.cpp
 *
 *  Created on: Feb 13, 2012
 *	  Author: hxin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <mmintrin.h>

#include "Common.h"
#include "bit_mask.h"

long long int success[7];
long long int fail[7];

void bitmask_init() {
	int i = 0;
	for (i = 0; i < 7; i++) {
		success[i] = 0;
		fail[i] = 0;
	}
	total_bit_mask_success = 0;
	total_binary_success = 0;
	total_bit_mask_success = 0;
	total_filtering = 0;
	total_binary_num = 0;
	total_edit_num = 0;
	total_pass_num = 0;
}

void load_mask(int hash_table_num, double* BitMaskTime) {
	double startTime = getTime();
	int i = 0;
	int j = 0;

	char filename[30];
	int bm_size;
	int tmp = 0;
	int raw_data;
	FILE * data_file;

	sprintf(filename, "entry_bm_%d", hash_table_num);
	data_file = fopen(filename, "r");
	for (i = 0; i < INDEX_NUM; i++) {
		tmp = fscanf(data_file, "%i", &raw_data);
		entry_bm[i].level =  raw_data;
		tmp = fscanf(data_file, "%i", &(entry_bm[i].index));
	}
	fclose(data_file);

	sprintf(filename, "third_bm_%d", hash_table_num);
	data_file = fopen(filename, "r");
	tmp = fscanf(data_file, "%i", &bm_size);
	third_bm = (third_mask*) malloc(sizeof(third_mask)*bm_size);
	for (i = 0; i < bm_size; i++) {
		for (j = 0; j < 2048; j++) {
			tmp = fscanf(data_file, "%i", &raw_data);
			third_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	fclose(data_file);

	sprintf(filename, "forth_bm_%d", hash_table_num);
	data_file = fopen(filename, "r");
	tmp = fscanf(data_file, "%i", &bm_size);
	forth_bm = (forth_mask*) malloc(sizeof(forth_mask)*bm_size);
	for (i = 0; i < bm_size; i++) {
		for (j = 0; j < 16384; j++) {
			tmp = fscanf(data_file, "%i", &raw_data);
			forth_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	fclose(data_file);

	sprintf(filename, "fifth_bm_%d", hash_table_num);
	data_file = fopen(filename, "r");
	tmp = fscanf(data_file, "%i", &bm_size);
	fifth_bm = (fifth_mask*) malloc(sizeof(fifth_mask)*bm_size);
	for (i = 0; i < bm_size; i++) {
		for (j = 0; j < 131072; j++) {
			tmp = fscanf(data_file, "%i", &raw_data);
			fifth_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	fclose(data_file);

	sprintf(filename, "sixth_bm_%d", hash_table_num);
	data_file = fopen(filename, "r");
	tmp = fscanf(data_file, "%i", &bm_size);
	sixth_bm = (sixth_mask*) malloc(sizeof(sixth_mask)*bm_size);
	for (i = 0; i < bm_size; i++) {
		for (j = 0; j < 1048576; j++) {
			tmp = fscanf(data_file, "%i", &raw_data);
			sixth_bm[i].mask[j] = (unsigned char) raw_data;
		}
	}
	fclose(data_file);
	*BitMaskTime = getTime() - startTime;
}

void free_mask() {
	//printf("free mask called\n");
	free(third_bm);
	free(forth_bm);
	free(fifth_bm);
	free(sixth_bm);
}

int test_mask(int target_coor, int hash_val) {
	//printf("test_mask target_coor:%d  hash_val:%d 	", target_coor, hash_val);
	if (hash_val == -1) {
		return 0;
	}
	else if (entry_bm[hash_val].level == ZERO) {
		//printf("test1");
		return 0;
	}
	else {
		//printf("test2");
		int lower_cover = 0;
		int upper_cover = 0;
		int lower_bound = target_coor - errThreshold;
		int upper_bound = target_coor + errThreshold;
		if (lower_bound < 0)
			lower_bound = 0;
		//printf("test3");

		int index = entry_bm[hash_val].index;
		//printf("test4");

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
			return 1;
		} else {
			success[entry_bm[hash_val].level-1]++;
			return 0;
		}
	}
}

long long int get_eval_data(int is_success, int idx) {
	if (is_success)
		return success[idx];
	else
		return fail[idx];
}

int over_fifth(int hash_val) {
	if (entry_bm[hash_val].level >= 5)
		return 1;
	else
		return 0;
}

int get_level(int hash_val) {
	return (int) entry_bm[hash_val].level;
}

void record_fail(int hash_val) {
	fail[entry_bm[hash_val].level - 1]++;
}
