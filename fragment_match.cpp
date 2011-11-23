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
#include "fragment_match.h"

int* hash_table;
int* coordinate;

void loadHash(string hash_name) {
	hashReconstructor(&hash_table, &coordinate, hash_name.c_str());
}

bool searchKey(int* start_coor, int target_coor, int entry_coor, int entry_size) {
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

	if (coordinate[mid] <= target_coor + max_indel_num && coordinate[mid]
			>= target_coor - max_indel_num) {
		*start_coor = coordinate[mid];
		return true;
	} else
		return false;
}

bool searchPrevious(int coor_value, int start_key_entry,
		list<match_result> previous_result) {
	if (previous_result.size() == 0) {
		return false;
	}
	for (list<match_result>::iterator it_coor = previous_result.begin(); it_coor
			!= previous_result.end(); ++it_coor) {
		if (((*it_coor).coordinate == coor_value - start_key_entry * KEY_LENGTH)) {
			return true;
		}
	}
	return false;
}

bool sortPrefilter(key_struct* sort_result, key_struct* sort_input) {
	if (select_cheapest == false) {
		for (int i = 0; i < max_diff_num + 1; i++) {
			sort_result[i].key_number = sort_input[i].key_number;
			sort_result[i].key_entry = sort_input[i].key_entry;
			sort_result[i].key_entry_size = sort_input[i].key_entry_size;
		}
		return true;
	}
	for (int i = 0; i < KEY_NUMBER; i++) {
		for (int j = 0; j < KEY_NUMBER; j++) {
			if (sort_input[i].key_entry_size > sort_input[j].key_entry_size) {
				sort_input[i].order = sort_input[i].order + 1;
			}
		}
	}
	int loop_index = 0;
	for (int i = 0; i < max_diff_num + 1; i++) {
		for (int j = 0; j < KEY_NUMBER; j++) {
			if (sort_input[j].order == i) {
				sort_result[loop_index].key_entry = sort_input[j].key_entry;
				sort_result[loop_index].key_number = sort_input[j].key_number;
				sort_result[loop_index].key_entry_size
						= sort_input[j].key_entry_size;
				loop_index = loop_index + 1;
			}
			if (loop_index == max_diff_num + 1)
				return true;
		}
	}
	return false;
}

list<match_result> searchFragment(string fragment) {
	list<match_result> result;
	key_struct sort_input[KEY_NUMBER];
	for (int i = 0; i < KEY_NUMBER; i++) {
		string key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
		int key_hash = hashVal(key);
		int key_entry = hash_table[key_hash];
		int key_entry_size = coordinate[key_entry];
		sort_input[i].order = 0;
		sort_input[i].key_number = i;
		sort_input[i].key_entry = key_entry;
		sort_input[i].key_entry_size = key_entry_size;
	}

	key_struct keys_input[max_diff_num + 1];
	sortPrefilter(keys_input, sort_input);

	for (int k = 0; k < max_diff_num + 1; k++) {
		for (int i = keys_input[k].key_entry + 1; i <= keys_input[k].key_entry
				+ keys_input[k].key_entry_size; i++) {
			int coor_value = coordinate[i];
			int diff_num = 0;
			int tmp_start_coor = 0;
			int start_coor = 0;
			bool first_matched = false;
			if (!searchPrevious(coor_value, keys_input[k].key_number, result)) {
				for (int j = 0; j < KEY_NUMBER; j++) {
					if (j - diff_num > KEY_NUMBER - max_diff_num)
						break;
					string segment_str = fragment.substr(j * KEY_LENGTH,
							KEY_LENGTH);
					if (!searchKey(
							&tmp_start_coor,
							coor_value + (j - keys_input[k].key_number)
									* KEY_LENGTH, sort_input[j].key_entry,
							sort_input[j].key_entry_size)) {
						diff_num++;
						if (diff_num > max_diff_num)
							break;
					} else if (first_matched == false) {
						start_coor = tmp_start_coor - j * KEY_LENGTH;
						first_matched = true;
					}
				}
				if (diff_num <= max_diff_num) {
					match_result temp;
					temp.coordinate = start_coor;
					temp.relevance = diff_num;
					temp.key_number = keys_input[k].key_number;
					result.push_back(temp);
				}
			}
		}
	}
	return result;
}

list<match_result> noFilterSearch(string fragment) {
	list<match_result> result;
	key_struct sort_input[KEY_NUMBER];
	for (int i = 0; i < KEY_NUMBER; i++) {
		string key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
		int key_hash = hashVal(key);
		int key_entry = hash_table[key_hash];
		int key_entry_size = coordinate[key_entry];
		sort_input[i].order = 0;
		sort_input[i].key_number = i;
		sort_input[i].key_entry = key_entry;
		sort_input[i].key_entry_size = key_entry_size;
	}

	key_struct keys_input[max_diff_num + 1];
	sortPrefilter(keys_input, sort_input);

	for (int k = 0; k < max_diff_num + 1; k++) {
		for (int i = keys_input[k].key_entry + 1; i <= keys_input[k].key_entry
				+ keys_input[k].key_entry_size; i++) {
			if (!searchPrevious(coordinate[i], keys_input[k].key_number, result)) {
				match_result temp;
				temp.coordinate = coordinate[i];
				temp.relevance = 0;
				temp.key_number = keys_input[k].key_number;
				result.push_back(temp);
			}
		}
	}
	return result;
}

int binary_filtering_cost(string fragment) {
	int result = 0;
	int status = 0;
	key_struct sort_input[KEY_NUMBER];
	for (int i = 0; i < KEY_NUMBER; i++) {
		string key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
		int key_hash = hashVal(key);
		int key_entry = hash_table[key_hash];
		int key_entry_size = coordinate[key_entry];
		sort_input[i].order = 0;
		sort_input[i].key_number = i;
		sort_input[i].key_entry = key_entry;
		sort_input[i].key_entry_size = key_entry_size;
	}

	key_struct keys_input[max_diff_num + 1];
	sortPrefilter(keys_input, sort_input);

	if (select_cheapest == true)
		sortPrefilter(keys_input, sort_input);
	else {
		for (int i = 0; i < max_diff_num + 1; i++) {
			keys_input[i].key_number = sort_input[i].key_number;
			keys_input[i].key_entry = sort_input[i].key_entry;
			keys_input[i].key_entry_size = sort_input[i].key_entry_size;
		}
	}

	for (int k = 0; k < max_diff_num + 1; k++) {
		result += keys_input[k].key_entry_size;
	}
	return result;
}
