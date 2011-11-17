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

void loadHash(string hash_name) {
	char * temp = new char [hash_name.size() + 1];
	hash_name.copy(temp, hash_name.size(), 0);
	hashReconstructorChar(&hash_table, &coordinate, &prefilter, temp);
	delete [] temp;
}

bool searchKey(string key, int target_coor) {
	int hash_value = hashVal(key);
	int entry_coor = hash_table[hash_value];
	int entry_size = coordinate[entry_coor];

	if (entry_size == 0) 
		return  false;

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
			return true;
		}
	}
	return false;
}

bool sortPrefilter(key_struct* sort_result, key_struct* sort_input) {
	int loop_index = 0;
	for (int i = 0; i < KEY_NUMBER; i++) {
		for(int j = 0; j < KEY_NUMBER; j++) {
			if (sort_input[i].key_entry_size > sort_input[j].key_entry_size) {
				sort_input[i].order = sort_input[i].order + 1;
			} 
		}
	}
	for (int i = 0; i < max_diff_num+1; i++) {
		for (int j = 0; j < KEY_NUMBER; j++){
			if (sort_input[j].order == i) {
				sort_result[loop_index].key_entry      = sort_input[j].key_entry;
				sort_result[loop_index].key_number     = sort_input[j].key_number;
				sort_result[loop_index].key_entry_size = sort_input[j].key_entry_size;
				loop_index = loop_index + 1;	
			}
			if (loop_index == max_diff_num + 1){
				return true;
			} 
		} 
	}
	return false;
}

list<match_result> searchFragment(string fragment) {
	list<match_result> result;
	int  start_key_entry[max_diff_num + 1];
	int  status = 0;
	key_struct sort_input[KEY_NUMBER]; 
	for(int i = 0; i < KEY_NUMBER; i++) {
		string key = fragment.substr(KEY_LENGTH*i, KEY_LENGTH);
		int key_hash  = hashVal(key);
		int key_entry = hash_table[key_hash];
		int key_entry_size = coordinate[key_entry];
		sort_input[i].order = 0;
		sort_input[i].key_number = i;
		sort_input[i].key_entry = key_entry;
		sort_input[i].key_entry_size = key_entry_size;
	}

	key_struct keys_input[max_diff_num+1]; 
	sortPrefilter(keys_input, sort_input);

	for(int i = 0 ; i < max_diff_num+1; i++){
		if (select_cheapest == true) 	
			start_key_entry[i] = keys_input[i].key_number;
		else 				
			start_key_entry[i] = i;
	}

	for (int k = 0; k < max_diff_num + 1; k++) {
		for (int i = keys_input[k].key_entry + 1; i <= keys_input[k].key_entry + keys_input[k].key_entry_size; i++) {
			int coor_value = coordinate[i];
			int diff_num = 0;
			if (!searchPrevious(coor_value, start_key_entry[k], result)){
				for (int j = 0; j < KEY_NUMBER; j++) {
					string segment_str = fragment.substr(j * KEY_LENGTH, KEY_LENGTH);
					if(!searchKey(segment_str, coor_value + (j-start_key_entry[k]) * KEY_LENGTH)) 
						diff_num++;
					else if (diff_num > max_diff_num)
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
