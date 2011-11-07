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
int* prefilter;

void loadHash(string hash_name) {
	char * temp = new char [hash_name.size() + 1];
	hash_name.copy(temp, hash_name.size(), 0);
	hashReconstructorChar(&hash_table, &coordinate, &prefilter, temp);
	delete [] temp;
}

bool searchPrefilter(string key) {
	int hash_value = hashVal(key);
	int entry_size = prefilter[0];

	int lower_bound = 1;
	int upper_bound = entry_size;
	int mid = lower_bound + entry_size / 2;
	while (lower_bound < upper_bound) {
		if (prefilter[mid] == hash_value)
			break;
		else if (prefilter[mid] < hash_value)
			lower_bound = mid + 1;
		else
			upper_bound = mid - 1;
		mid = lower_bound + (upper_bound - lower_bound) / 2;
	}

	if (prefilter[mid] == hash_value) 
		return true;
	else
		return false;
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

	//assert(mid > entry_coor && mid <= entry_coor + entry_size);

	if (coordinate[mid] <= target_coor + max_indel_num
			&& coordinate[mid] >= target_coor - max_indel_num)
		return true;
	else
		return false;
}

list<match_result> searchFragment(string fragment) {
	list<match_result> result;
	int key_number = fragment.size() / KEY_LENGTH;
	string key;
	int key_hash; 
	int key_entry;
	int key_entry_size;
	int start_key_entry = 0;
	
////	for (int i = 0; i < key_number; i++) {
//		key = fragment.substr(KEY_LENGTH*start_key_entry, KEY_LENGTH);
//		key_hash  = hashVal(key);
//		key_entry = hash_table[key_hash];
//		key_entry_size = coordinate[key_entry];
////		if (key_entry_size <= max_entry_size){
//			break;
//		} else {
//			start_key_entry++;
//		}
//	}
	for (int i = 0; i < key_number; i++){
//		key = fragment.substr(KEY_LENGTH*start_key_entry, KEY_LENGTH);
		key = fragment.substr(KEY_LENGTH*i, KEY_LENGTH);
		if(!searchPrefilter(key)) {
			start_key_entry = i;
			break;
		}
	}

	cout << "key number:" << key_number << "  start key entry :" << start_key_entry;
	key = fragment.substr(KEY_LENGTH*start_key_entry, KEY_LENGTH);
	key_hash  = hashVal(key);
	key_entry = hash_table[key_hash];
	key_entry_size = coordinate[key_entry];

	for (int i = key_entry + 1; i <= key_entry + key_entry_size; i++) {
		int coor_value = coordinate[i];
		int diff_num = 0;
		for (int j = 0; j < key_number; j++) {
			string segment_str = fragment.substr(j * KEY_LENGTH, KEY_LENGTH);
			if (!searchKey(segment_str, coor_value + (j-start_key_entry) * KEY_LENGTH) ){
				diff_num++;
			}
			else
			if (diff_num > max_diff_num)
				break;
		}
		if (diff_num <= max_diff_num) {
			match_result temp;
			temp.coordinate = coor_value;
			temp.relevance = diff_num;
			result.push_back(temp);
		}
	}
	return result;
}

//This function is a naive version,
//list<match_result> searchFragment(string fragment) {
//	list<match_result> result;
//	int key_number = fragment.size() / KEY_LENGTH;
//	int start_key_entry = 0;
//	string key;
//
//	for (int i = 0; i < key_number; i++){
//		key = fragment.substr(KEY_LENGTH*i, KEY_LENGTH);
//		if(!searchPrefilter(key)) {
//			start_key_entry = i;
//			break;
//		}
//	}
//	cout << "key number:" << key_number << "  start key entry :" << start_key_entry << endl;
//	key = fragment.substr(KEY_LENGTH*start_key_entry, KEY_LENGTH);
//
//	int key_hash = hashVal(key);
//	int key_entry = hash_table[key_hash];
//	int key_entry_size = coordinate[key_entry];
//	for (int i = key_entry + 1; i <= key_entry + key_entry_size; i++) {
//		int coor_value = coordinate[i];
//		int diff_num = 0;
//		for (int j = 0; j < key_number; j++) {
//			string segment_str = fragment.substr(j * KEY_LENGTH, KEY_LENGTH);
//			if (!searchKey(segment_str, coor_value + (j-start_key_entry) * KEY_LENGTH) ){
//				diff_num++;
//			}
//			else
//			if (diff_num > max_diff_num)
//				break;
//		}
//		if (diff_num <= max_diff_num) {
//			match_result temp;
//			temp.coordinate = coor_value;
//			temp.relevance = diff_num;
//			result.push_back(temp);
//		}
//	}
//	return result;
//}

