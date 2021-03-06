/*
 * fragment_match.h
 *
 *  Created on: Oct 7, 2011
 *      Author: mac
 */

#ifndef FRAGMENT_MATCH_H_
#define FRAGMENT_MATCH_H_

#define MAX_UPPER_BOUND 10
#define MIN_LOWER_BOUND 2

using namespace std;

struct match_result {
	int coordinate;
	int mismatch_num;
	int insersion_num;
	int deletion_num;
	int relevance;
	bool correct;
	int key_number;
};

struct key_struct {
	int key_entry;
	int key_entry_size;
	int key_number;
	int order;
	int base;
};

struct GPU_fragment {
	char	fragment[READ_LENGTH + 1];
	key_struct	sorted_keys[KEY_NUMBER];
};

struct coor_result {
	int			coordiante;
	int			diff_num;
	ED_error	error[MAX_ERROR_NUM];
};

struct final_result {
	char		fragment[READ_LENGTH + 1];
	bool		spilled;
	int			size;
	coor_result	coor_results[MAX_COOR_RESULT_NUM];
};

/*
 * Function:		Search the possible locations and calculate their relevance.
 * Input:			fragment is the target gene sequence need to be mapped.
 * Modification:	result will be the top RETURN_RESULT_NUM of results that could possibly
 * 					be the mapping location.
 *
 * Input:			fragment:	the fragment set
 * 					size:		fragment set size
 * 					ref:		reference string
 * 					hash_table:	hash table
 * 					coordinate:	coordinate table
 * 					max_diff_num: maximum different tolerance
 * 					max_indel_num: maximum insertions or deletions
 * Output:			result: the final results.
 */
bool sortPrefilter(key_struct* sort_result, key_struct* sort_input);

__global__ void searchFragment(GPU_fragment* fragment, int size, char* ref,
		int* hash_table, int* coordiante, int max_diff_num,
		int max_indel_num, final_result* result);

__device__ bool searchKey(int target_coor, int entry_coor, int entry_size,
		int* coordinate);

#include "fragment_match.cu"
#endif /* FRAGMENT_MATCH_H_ */
