/*
 * bitmask.h
 *
 *  Created on: Feb 8, 2012
 *      Author: hxin
 */

#ifndef BITMASK_H_
#define BITMASK_H_

#define KEY_LENGTH 12
#define INDEX_NUM  (1 << (2 * KEY_LENGTH ) )

enum bm_level {ZERO = 0, FIRST = 1, SECOND, THIRD, FORTH, FIFTH, SIXTH};

typedef struct {
	int level;
	unsigned int index;
} bm_info;

typedef struct {
	unsigned char mask[16];
} first_mask;

typedef struct {
	unsigned char mask[64];
} second_mask;

typedef struct {
	unsigned char mask[2048];
} third_mask;

typedef struct {
	unsigned char mask[16384];
} forth_mask;

typedef struct {
	unsigned char mask[131072];
} fifth_mask;

typedef struct {
	unsigned char mask[1048576];
} sixth_mask;

void	load_mask(int hash_table_num, double* BitMaskTime);
void	free_mask();

int		test_mask(int target_coor, int hash_val);
int		over_fifth(int hash_val);

void	record_fail(int hash_val);
int		get_level(int hash_val);
long long int	get_eval_data(int is_success, int idx);

bm_info entry_bm[INDEX_NUM];
first_mask * first_bm;
second_mask * second_bm;
third_mask* third_bm;
forth_mask* forth_bm;
fifth_mask* fifth_bm;
sixth_mask* sixth_bm;

long long int total_bit_mask_success;
long long int total_binary_success;
long long int total_bit_mask_success;
long long int total_filtering;
long long int total_binary_num;
long long int total_edit_num;
long long int total_pass_num;
#endif /* BITMASK_H_ */
