/*
 * fragment_match.cpp
 *
 *  Created on: Oct 8, 2011
 *      Author: mac
 */
#include <cstdlib>
#include <sstream>
#include "fragment_match.h"

void loadHash(string hash_name) {
	hashReconstructorChar(&hash_table, &coordinate, hash_name.str().c_str());
}

bool searchKey(string key, int target_coor) {
	//locate the entry
	int result_coor = -1;
	int hash_value = hashVal(key);
	int entry_coor = hash_table[hash_value];
	//get entry size
	int entry_size = coordinate[result_idx][result_coor];

	//in the case the entry is empty, we can't find it.
	if (entry_size == 0)
		return false;

	//otherwise, start binary search
	//define lower and upper bound
	int lower_bound = entry_coor + 1;
	int upper_bound = entry_coor + entry_size;
	int mid = lower_bound + entry_size / 2;
	while (lower < upper) {
		if (coordinate[result_idx][mid] == target_coor)
			break;
		else if (coordinate[result_idx][mid] < target_coor)
			lower_bound = mid + 1;
		else
			upper_bound = mid - 1;
		mid = lower_bound + (upper_bound - lower_bound) / 2;
	}

	assert(mid > entry_coor && mid <= entry_coor + entry_size);

	if (result_coor <= target_coor + max_indel_num
			&& result_coor >= target_coo - max_indel_num)
		return true;
	else
		return false;
}

//This function is a naive version,
bool searchFragment(list<match_result> &result, string fragment) {
	int key_number = fragment.size() / KEY_LENGTH;
	string key;
	key = fragment.substr(0, KEY_LENGTH);
	int key_idx = hashIdx(key);
	int key_hash = hashValue(key);
	int key_entry = hash_table[key_idx][key_hash];
	int key_entry_size = coordinate[key_idx][key_entry];

	//start probing the key entry in hash table and calculate the
	for (int i = key_entry + 1; i <= key_entry + key_entry_size; i++) {
		int coor_value = coordinate[key_idx][i];
		int match_up_num = 1;
		for (int j = 1; j < key_number; j++) {
			string segment_str = fragment.substr(j * KEY_LENGTH, KEY_LENGTH);
			if (searchKey(segment_str, coor_value) )
				match_up_num++;
		}
		if (match_up_num >= key_number + max_diff_num) {
			cout << "found_match at: " << coor_value << endl;
			match_result temp;
			temp.coordinate = coor_value;
			temp.relevance = key_number - match_up_num;
			result.push_back(temp);
		}
	}
}

