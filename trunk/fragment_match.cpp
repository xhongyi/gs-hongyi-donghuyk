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
#include "fragment_match.h"

int* hash_table;

int* coordinate;

void loadHash(string hash_name) {
	char * temp = new char [hash_name.size() + 1];
	hash_name.copy(temp, hash_name.size(), 0);
	hashReconstructorChar(&hash_table, &coordinate, temp);
	delete [] temp;
}

bool searchKey(string key, int target_coor) {
	//locate the entry
	int hash_value = hashVal(key);
	int entry_coor = hash_table[hash_value];
	//get entry size
	int entry_size = coordinate[entry_coor];

	//in the case the entry is empty, we can't find it.
	if (entry_size == 0)
		return false;

	//otherwise, start binary search
	//define lower and upper bound
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

	assert(mid > entry_coor && mid <= entry_coor + entry_size);

	if (coordinate[mid] <= target_coor + max_indel_num
			&& coordinate[mid] >= target_coor - max_indel_num)
		return true;
	else
		return false;
}

//This function is a naive version,
list<match_result> searchFragment(string fragment) {
	list<match_result> result;
	int key_number = fragment.size() / KEY_LENGTH;
	string key;
	key = fragment.substr(0, KEY_LENGTH);
	int key_hash = hashVal(key);
	int key_entry = hash_table[key_hash];
	int key_entry_size = coordinate[key_entry];

	//start probing the key entry in hash table and calculate the
	for (int i = key_entry + 1; i <= key_entry + key_entry_size; i++) {
		int coor_value = coordinate[i];
		int diff_num = 0;
		for (int j = 1; j < key_number; j++) {
			string segment_str = fragment.substr(j * KEY_LENGTH, KEY_LENGTH);
			if (!searchKey(segment_str, coor_value + j * KEY_LENGTH) )
				diff_num++;
			else
				cout << "j" << j << endl;
			if (diff_num > max_diff_num)
				break;
		}
		cout << "Hello!!!!" << endl;
		cout << "Diffnumber: " << diff_num << endl;
		cout << "max_diff_num: " << max_diff_num << endl;
		if (diff_num <= max_diff_num) {
			cout << "found_match at: " << coor_value << endl;
			match_result temp;
			temp.coordinate = coor_value;
			temp.relevance = diff_num;
			result.push_back(temp);
		}
	}
	return result;
}

