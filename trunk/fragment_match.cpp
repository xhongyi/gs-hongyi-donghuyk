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
int* prefilter;

list<sort_result> sortPrefilter(string fragment); 
bool sort_function (sort_result first, sort_result second);

void loadHash(string hash_name) {
	char * temp = new char [hash_name.size() + 1];
	hash_name.copy(temp, hash_name.size(), 0);
	hashReconstructorChar(&hash_table, &coordinate, &prefilter, temp);
	delete [] temp;
}

bool sort_function (sort_result first, sort_result second) {
	unsigned int i = 0;
	if (first.key_entry_size < second.key_entry_size) return true;
	else if (first.key_entry_size == second.key_entry_size) {
		if (first.key_number < second.key_number) return true;
		else return false;
	} 
	else return false;
}

list<sort_result> sortPrefilter(string fragment) {
	list<sort_result> result;
	sort_result tmp;
	string key;
	int key_hash; 
	int key_entry;
	int key_entry_size;
	for (int i = 0; i < KEY_NUMBER; i++) {
		key = fragment.substr(KEY_LENGTH*i, KEY_LENGTH);
		key_hash  = hashVal(key);
		key_entry = hash_table[key_hash];
		key_entry_size = coordinate[key_entry];
		tmp.key_number = i;
		tmp.key_entry_size = key_entry_size;
		result.push_back(tmp);
	}
	result.sort(sort_function);
	return result;
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
		return  false;

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

	if (coordinate[mid] <= target_coor + max_indel_num && coordinate[mid] >= target_coor - max_indel_num) {
		return true;
	} else {
		return false;
	}
}

bool searchPrevious(int coor_value, int start_key_entry, list<match_result> previous_result) {
	if (previous_result.size() == 0) {
		return false;
	} 
	for(list<match_result>::iterator it_coor=previous_result.begin(); it_coor != previous_result.end(); ++it_coor) {
		int upper_bound = coor_value - start_key_entry*KEY_LENGTH + max_indel_num;
		int lower_bound = coor_value - start_key_entry*KEY_LENGTH - max_indel_num;
		if(((*it_coor).coordinate <=  upper_bound)&&((*it_coor).coordinate >= lower_bound)  ){
//			cout << "matched!!" << endl;
			return true;
		}
	}
//	cout << "unmatched!!" << endl;
	return false;
}

list<match_result> searchFragment(string fragment) {
	list<match_result> result;
	int  start_key_entry[max_diff_num + 1];
	int  status = 0;
	list<sort_result> sorted_result = sortPrefilter(fragment);
	for(list<sort_result>::iterator it_sort=sorted_result.begin(); it_sort !=sorted_result.end(); ++it_sort) {
		if (select_cheapest == true) {
			start_key_entry[status] = (*it_sort).key_number;	
		} else {
			start_key_entry[status] = status;
		}
		status = status + 1;
	}
	for (int k = 0; k < max_diff_num + 1; k++) {
		string key = fragment.substr(KEY_LENGTH*start_key_entry[k], KEY_LENGTH);
		int key_hash  = hashVal(key);
		int key_entry = hash_table[key_hash];
		int key_entry_size = coordinate[key_entry];
	
		for (int i = key_entry + 1; i <= key_entry + key_entry_size; i++) {
			int coor_value = coordinate[i];
			int diff_num = 0;
			if (!searchPrevious(coor_value, start_key_entry[k], result)){
				for (int j = 0; j < KEY_NUMBER; j++) {
					string segment_str = fragment.substr(j * KEY_LENGTH, KEY_LENGTH);
					if(!searchKey(segment_str, coor_value + (j-start_key_entry[k]) * KEY_LENGTH)) {
						diff_num++;
					} 
					else 
					if (diff_num > max_diff_num)
						break;
				}
				if (diff_num <= max_diff_num) {
					match_result temp;
					temp.coordinate = coor_value-start_key_entry[k]*KEY_LENGTH;
					temp.relevance = diff_num;
					result.push_back(temp);
				}	
			}
		}
	}
	return result;
}
