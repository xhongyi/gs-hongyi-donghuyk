/*
 * fragment_match.cpp
 *
 *  Created on: Oct 8, 2011
 *      Author: mac
 */
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <assert.h>
#include "common.h"
#include "edit_distance.h"
#include "fragment_match.h"

bool select_cheapest;

__device__ bool searchKey(int target_coor, int entry_coor, int entry_size,
		int* coordinate, int max_indel_num);

__device__ bool searchKey(int target_coor, int entry_coor, int entry_size,
		int* coordinate, int max_indel_num) {
	printf("1\n");
	if (entry_size == 0)
		return false;
	int lower_bound = entry_coor + 1;
	int upper_bound = entry_coor + entry_size;
	int mid = lower_bound + entry_size / 2;
	while (lower_bound < upper_bound) {
		if (coordinate[mid] == target_coor)
			break;
		else if (coordinate[mid] < target_coor)
			lower_bound = mid + 1;
		else
			upper_bound = mid - 1;
		mid = lower_bound + (upper_bound - lower_bound) / 2;
	}
	printf("2\n");

	printf("mid: %i\n", mid);
	printf("coordinate[mid]: %i\n", coordinate[mid]);

	if (coordinate[mid] <= target_coor + max_indel_num && coordinate[mid]
			>= target_coor - max_indel_num) {
		printf("3 true\n");
		return true;
	} else {
		printf("3 false\n");
		return false;
	}
}

bool sortPrefilter(key_struct* sort_result, key_struct* sort_input) {
	for (int i = 0; i < KEY_NUMBER; i++) {
		for (int j = 0; j < KEY_NUMBER; j++) {
			if (sort_input[i].key_entry_size > sort_input[j].key_entry_size) {
				sort_input[i].order = sort_input[i].order + 1;
			}
		}
	}
	int accumulative_base = 0;
	int loop_index = 0;
	for (int i = 0; i < KEY_NUMBER; i++) {
		for (int j = 0; j < KEY_NUMBER; j++) {
			if (sort_input[j].order == i) {
				sort_result[loop_index].key_entry = sort_input[j].key_entry;
				sort_result[loop_index].key_number = sort_input[j].key_number;
				sort_result[loop_index].key_entry_size
						= sort_input[j].key_entry_size;
				sort_result[loop_index].base = accumulative_base;
				accumulative_base += sort_result[loop_index].key_entry_size;
				loop_index = loop_index + 1;
			}
			//if (loop_index == max_diff_num + 1)
			//	return true;
		}
	}
	return true;//false;
}

__global__ void searchFragment(GPU_fragment* fragment, int fragment_size,
		char* ref, int* hash_table, int* coordinate, int max_diff_num,
		int max_indel_num, final_result* result) {
	printf("HELLO!!\n");
	printf("cuda fragment: %c\n", (*fragment).fragment[0]);
	printf("blockIdx.x: %i, threadIdx.x: %i\n", blockIdx.x, threadIdx.x);
	//This will be used in edit_distance Calculation.
	int main_lane = max_indel_num + 1;
	//Each thread will have a path array for edit_distance calculation.
	ED_path path[MAX_ERROR_NUM];

	//Fragment_counter: get cooresponding fragment
	__shared__ int fragment_count;
	__shared__ int size;
	if (threadIdx.x == 0) {
		printf("Hey Man!!! Here is threadIdx.x == 0\n");
		fragment_count = blockIdx.x;
	}

	while (fragment_count < fragment_size) {
		printf("Ever started!!\n");
		//get the corresponding key_num and it's coordinate.
		int coor_count = threadIdx.x;
		int cur_key = 0;

		if (threadIdx.x == 0) {
			size = 0;
		}

		do {
			printf("Hey doing something!\n");
			//Do adjacency filtering
			int diff_num = 0;
			int
					coor_idx =
							fragment[fragment_count].sorted_keys[cur_key].key_entry
									+ 1 + coor_count
									- fragment[fragment_count].sorted_keys[cur_key].base;
			printf("fragment[fragment_count].sorted_keys[cur_key].key_entry: %i\n", fragment[fragment_count].sorted_keys[cur_key].key_entry);
			printf("fragment[fragment_count].sorted_keys[cur_key].base: %i\n", fragment[fragment_count].sorted_keys[cur_key].base);
			printf("fragment[fragment_count].sorted_keys[cur_key].key_entry_size: %i\n", fragment[fragment_count].sorted_keys[cur_key].key_entry_size);
			printf("***fragment_count: %i, cur_key: %i, coor_count: %i, coor_idx: %i\n", fragment_count, cur_key,coor_count, coor_idx);

			for (int i = 0; i < KEY_NUMBER; i++) { //for each segment
				if (i - diff_num > KEY_NUMBER - max_diff_num)
					break;

				printf(
						"coor_idx + (fragment[fragment_count].sorted_keys[i].key_number - fragment[fragment_count].sorted_keys[cur_key].key_number) * KEY_LENGTH: %i\n",
						(coordinate[coor_idx]
								+ (fragment[fragment_count].sorted_keys[i].key_number
										- fragment[fragment_count].sorted_keys[cur_key].key_number)
										* KEY_LENGTH));
				printf(
						"fragment[fragment_count].sorted_keys[i].key_entry: %i\n",
						fragment[fragment_count].sorted_keys[i].key_entry);
				printf(
						"fragment[fragment_count].sorted_keys[i].key_entry_size: %i\n",
						fragment[fragment_count].sorted_keys[i].key_entry_size);

				if (!searchKey(
						coordinate[coor_idx]
								+ (fragment[fragment_count].sorted_keys[i].key_number
										- fragment[fragment_count].sorted_keys[cur_key].key_number)
										* KEY_LENGTH,
						fragment[fragment_count].sorted_keys[i].key_entry,
						fragment[fragment_count].sorted_keys[i].key_entry_size,
						coordinate, max_indel_num)) {
					diff_num++;
					printf("4\n");
					if (diff_num > max_diff_num)
						break;
				}
			}
			printf("5\n");
			//Edit_distance Calculation
			if (diff_num <= max_diff_num) {
				printf("Hey in ED test now\n");
				printf("cur_key: %i\n", cur_key);
				char ref_str[READ_LENGTH];
				for (int i = 0; i < READ_LENGTH; i++) { //Get reference string
					printf(
							"coordinate: %i\n",
							coordinate[coor_idx]
									- fragment[fragment_count].sorted_keys[cur_key].key_number
											* KEY_LENGTH + i);
					ref_str[i]
							= ref[coordinate[coor_idx]
									- fragment[fragment_count].sorted_keys[cur_key].key_number
											* KEY_LENGTH + i];
					printf("ref_str[%i]: %c\n", i, ref_str[i]);
				}
				printf("7\n");

				ED_result
						edit_result =
								editDistanceCal(
										fragment[fragment_count].fragment,
										ref_str,
										fragment[fragment_count].sorted_keys[cur_key].key_number,
										path, main_lane, max_indel_num,
										max_diff_num);

				if (edit_result.correct) {
					printf("find something!\n");
					atomicAdd(&size, 1);
					result[fragment_count].coor_results[size - 1].coordiante
							= coordinate[coor_idx]
									- fragment[fragment_count].sorted_keys[cur_key].key_number
											* KEY_LENGTH;
					result[fragment_count].coor_results[size - 1].diff_num
							= edit_result.diff_num;
					for (int i = 0; i < edit_result.diff_num; i++)
						result[fragment_count].coor_results[size - 1].error[i]
								= edit_result.error[i];
				}
			}
			printf("6\n");
			//move to the next coordinate by incrementing coor_count
			printf("blockDim.x: %i\n", blockDim.x);
			coor_count += blockDim.x;
			//Move to the next key
			while (cur_key <= max_diff_num
					&& coor_count
							>= fragment[fragment_count].sorted_keys[cur_key].base
									+ fragment[fragment_count].sorted_keys[cur_key].key_entry_size) {
				printf("&&&cur_key incrementing!!\n");
				cur_key++;
			}
			printf("still here!!!\n");
		} while (size < MAX_COOR_RESULT_NUM && cur_key <= max_diff_num);

		printf("before syncthreads\n");

		__syncthreads();

		printf("print Ids again!! blockIdx.x: %i, threadIdx.x: %i\n",
				blockIdx.x, threadIdx.x);

		if (threadIdx.x == 0) {
			printf("I'm Here!!!\n");
			for (int i = 0; i < READ_LENGTH; i++) {
				result[fragment_count].fragment[i]
						= fragment[fragment_count].fragment[i];
				printf("result[%i].fragment[%i]: %c\n", fragment_count, i,
						result[fragment_count].fragment[i]);
			}
			if (size >= MAX_COOR_RESULT_NUM)
				result[fragment_count].spilled = true;
			else {
				result[fragment_count].spilled = false;
				result[fragment_count].size = size;
			}
			fragment_count += gridDim.x;
		}

	}
}

/*
 final_result searchFragment(char* fragment, char* ref, int* hash_table,
 int* coordinate) {
 key_struct sort_input[KEY_NUMBER];
 for (int i = 0; i < KEY_NUMBER; i++) {
 char key[KEY_LENGTH];
 for (int j = 0; j < KEY_NUMBER; j++) {
 key[j] = fragment[j + KEY_LENGTH * i];
 //key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
 }
 int key_hash = hashVal(key);
 int key_entry = hash_table[key_hash];
 int key_entry_size = coordinate[key_entry];
 sort_input[i].order = 0;
 sort_input[i].key_number = i;
 sort_input[i].key_entry = key_entry;
 sort_input[i].key_entry_size = key_entry_size;
 }

 key_struct keys_input[KEY_NUMBER];
 sortPrefilter(keys_input, sort_input);
 previous_result.size = 0;
 final_result return_result;
 return_result.total_binary_search = 0;
 return_result.total_edit_perform = 0;
 return_result.total_correct_num = 0;

 for (int k = 0; k < max_diff_num + 1; k++) { //Getting the seed
 for (int i = keys_input[k].key_entry + 1; i <= keys_input[k].key_entry
 + keys_input[k].key_entry_size; i++) { //Going through each entry in the seed coordinate list
 int coor_value = coordinate[i];
 int diff_num = 0;
 if (!searchPrevious(coor_value, keys_input[k].key_number,
 previous_result)) { //Pre-filtering
 return_result.total_binary_search++;
 for (int j = 0; j < KEY_NUMBER; j++) { //for each segment
 if (j - diff_num > KEY_NUMBER - max_diff_num)
 break;
 if (!searchKey(
 coor_value + (keys_input[j].key_number
 - keys_input[k].key_number) * KEY_LENGTH,
 keys_input[j].key_entry,
 keys_input[j].key_entry_size)) {
 diff_num++;
 if (diff_num > max_diff_num)
 break;
 }
 }
 if (diff_num <= max_diff_num) {
 if (previous_result.size <= PREFILTER_SIZE) {
 previous_result.coor[previous_result.size] = coor_value
 - keys_input[k].key_number * KEY_LENGTH; //start_coor;
 previous_result.size++;
 }
 return_result.total_edit_perform++;
 string ref_str(FRAGMENT_LENGTH, 'A');
 ref_str = (*ref).substr(
 coor_value - keys_input[k].key_number * KEY_LENGTH,
 FRAGMENT_LENGTH); //start_coor;

 /////////////////////Just For Testing
 char test_char[READ_LENGTH];
 char ref_char[READ_LENGTH];
 strcpy(test_char, fragment.c_str());
 strcpy(ref_char, ref_str.c_str());
 cout << "ref__read: " << ref_char << endl;
 cout << "test_read: " << test_char << endl;
 cout << "key_num__: " << keys_input[k].key_number << endl;
 /////////////////////Testing END

 ED_result edit_result = editDistanceCal(test_char,
 ref_char, keys_input[k].key_number);
 if (edit_result.correct) {
 return_result.total_correct_num++;
 //cout << "ref_read      : " << ref_str << "  coordinate: "<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
 //cout << "  result: correct " << endl;
 } else {
 //cout << "ref_read      : " << ref_str << "  coordinate: "<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
 //cout << "  result: not correct" <<endl;
 }
 }
 }
 }
 }
 return return_result;
 }

 */
