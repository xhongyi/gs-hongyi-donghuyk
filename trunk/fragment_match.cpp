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
#include <fstream>
#include "edit_distance.h"
#include "edit_distribution.h"
#include "fragment_match.h"

int* hash_table;
int* coordinate;
previous_coor previous_result;

void getHashTablePtr(int ** ptr) {
	*ptr = hash_table;
}

void getCoordinatePtr(int ** ptr) {
	*ptr = coordinate;
}

void loadHash(string hash_name) {
	hashReconstructor(&hash_table, &coordinate, hash_name.c_str());
}

void freeHash() {
	free(hash_table);
	free(coordinate);
}

bool searchKey(int target_coor, int entry_coor, int entry_size) {
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
		return true;
	} else
		return false;
}

bool searchPrevious(int coor_value, int start_key_entry,
		previous_coor previous_result) {
	if (previous_result.size == 0) {
		return false;
	}
	for (int i = 0; i < previous_result.size; i++) {
		if (previous_result.coor[i] > coor_value - start_key_entry * KEY_LENGTH
				- max_diff_num && previous_result.coor[i] < coor_value
				- start_key_entry * KEY_LENGTH + max_diff_num) {
			return true;
		}
	}
	return false;
}

bool sortPrefilter(key_struct* sort_result, key_struct* sort_input) {
	if (select_cheapest == false) {
		for (int i = 0; i < KEY_NUMBER; i++) {
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
	for (int i = 0; i < KEY_NUMBER; i++) {
		for (int j = 0; j < KEY_NUMBER; j++) {
			if (sort_input[j].order == i) {
				sort_result[loop_index].key_entry = sort_input[j].key_entry;
				sort_result[loop_index].key_number = sort_input[j].key_number;
				sort_result[loop_index].key_entry_size
						= sort_input[j].key_entry_size;
				loop_index = loop_index + 1;
			}
			//if (loop_index == max_diff_num + 1)
			//	return true;
		}
	}
	return true;//false;
}

bool sortPrefilterAllowN(key_struct* sort_result, key_struct* sort_input, int available_key_num) {
	if (select_cheapest == false) {
		for (int i = 0; i < available_key_num; i++) {
			sort_result[i].key_number = sort_input[i].key_number;
			sort_result[i].key_entry = sort_input[i].key_entry;
			sort_result[i].key_entry_size = sort_input[i].key_entry_size;
		}
		return true;
	}
	for (int i = 0; i < available_key_num; i++) {
		for (int j = 0; j < available_key_num; j++) {
			if (sort_input[i].key_entry_size > sort_input[j].key_entry_size) {
				sort_input[i].order = sort_input[i].order + 1;
			}
		}
	}
	int loop_index = 0;
	for (int i = 0; i < available_key_num; i++) {
		for (int j = 0; j < available_key_num; j++) {
			if (sort_input[j].order == i) {
				sort_result[loop_index].key_entry = sort_input[j].key_entry;
				sort_result[loop_index].key_number = sort_input[j].key_number;
				sort_result[loop_index].key_entry_size = sort_input[j].key_entry_size;
				sort_result[loop_index].order = sort_input[j].order;
				loop_index = loop_index + 1;
			}
			//if (loop_index == max_diff_num + 1)
			//	return true;
		}
	}
	return true;//false;
}

final_result searchFragment(string fragment, string* ref) {
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

	key_struct keys_input[KEY_NUMBER];
	sortPrefilter(keys_input, sort_input);
	previous_result.size = 0;
	final_result return_result;
	return_result.total_binary_search = 0;
	return_result.total_edit_perform = 0;
	return_result.total_correct_num = 0;

	for (int k = 0; k < max_diff_num + 1; k++) {
		for (int i = keys_input[k].key_entry + 1; i <= keys_input[k].key_entry
				+ keys_input[k].key_entry_size; i++) {
			int coor_value = coordinate[i];
			int diff_num = 0;
			if (!searchPrevious(coor_value, keys_input[k].key_number,
					previous_result)) {
				return_result.total_binary_search++;
				for (int j = 0; j < KEY_NUMBER; j++) {
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
					char test_char[READ_LENGTH + 1];
					char ref_char[READ_LENGTH + 1];
					strcpy(test_char, fragment.c_str());
					strcpy(ref_char, ref_str.c_str());
					/*////////////////////Testing END
					cout << "ref__read: " << ref_char << endl;
					cout << "test_read: " << test_char << endl;
					cout << "key_num__: " << keys_input[k].key_number << endl;
					*/
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

final_result searchFragment_fastq(string fragment, string* ref, ofstream * output_file, 
							char* contig_name, string fragment_name, string fragment_qual) {
	key_struct sort_input[KEY_NUMBER];
	int available_key_num = 0;
	for (int i = 0; i < KEY_NUMBER; i++) {
		string key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
		int key_hash = hashVal(key);
		if (key_hash >= 0) {									// check if there is 'N'
			int key_entry = hash_table[key_hash];
			int key_entry_size = coordinate[key_entry];
			sort_input[available_key_num].order = 0;
			sort_input[available_key_num].key_number = i;
			sort_input[available_key_num].key_entry = key_entry;
			sort_input[available_key_num].key_entry_size = key_entry_size;
			available_key_num++;
		}									
	}
	int n_num = KEY_NUMBER - available_key_num; 

	// Checking sorting input with N
	cout << "number of N  : " << n_num << endl;
	cout << "available key: " << available_key_num << endl;
	for (int i=0; i < available_key_num; i++) {
		cout 	<< "pre    key  " << i 
				<< "	" << sort_input[i].key_entry 
				<< "	" << sort_input[i].key_entry_size 
				<< "	" << sort_input[i].order 
				<< endl;
	}
	cout << endl;

	previous_result.size = 0;
	final_result return_result;
	return_result.total_binary_search = 0;
	return_result.total_edit_perform = 0;
	return_result.total_correct_num = 0;

	if (n_num > max_diff_num){
		return return_result;
	}
	int potential_diff_num = max_diff_num - n_num;

	key_struct keys_input[KEY_NUMBER];
	sortPrefilterAllowN(keys_input, sort_input, available_key_num); // DHL

	// Checking sorting input with N
	cout << "number of N  : " << n_num << endl;
	cout << "available key: " << available_key_num << endl;
	for (int i=0; i < available_key_num; i++) {
		cout 	<< "Sorted key  " << i 
				<< "	" << keys_input[i].key_entry 
				<< "	" << keys_input[i].key_entry_size 
				<< "	" << keys_input[i].order 
				<< endl;
	}
	cout << endl;
	// ---------- Allow N ---------------
	// KEY_NUMBER -> available_key_num
	// max_diff_num -> potential_diff_num
	// ----------------------------------

	int operating_key_num = max_diff_num + 1 - n_num;
	if (available_key_num < max_diff_num + 1 - n_num){
		operating_key_num = available_key_num;
	}

	// Checking operating key with N
	cout << "operating key: " << operating_key_num << endl;

	for (int k = 0; k < operating_key_num; k++) {
		for (int i = keys_input[k].key_entry + 1; i <= keys_input[k].key_entry
				+ keys_input[k].key_entry_size; i++) {
			int coor_value = coordinate[i];
			int diff_num = 0;
			if (!searchPrevious(coor_value, keys_input[k].key_number, previous_result)) {
				return_result.total_binary_search++;

				//-------------Binary Search---------------------------------------------------
				for (int j = 0; j < available_key_num; j++) {
					if (j - diff_num - n_num > KEY_NUMBER - max_diff_num)
						break;
					if (!searchKey( coor_value + (keys_input[j].key_number - keys_input[k].key_number) * KEY_LENGTH,
							keys_input[j].key_entry, keys_input[j].key_entry_size)) {
						diff_num++;
						if (diff_num + n_num > max_diff_num)
							break;
					}
				}
				//-----------------------------------------------------------------------------

				if (diff_num + n_num <= max_diff_num) {
					if (previous_result.size <= PREFILTER_SIZE) {
						previous_result.coor[previous_result.size] = coor_value
								- keys_input[k].key_number * KEY_LENGTH; //start_coor;
						previous_result.size++;
					}
					return_result.total_edit_perform++;
					string ref_str(FRAGMENT_LENGTH, 'A');
					ref_str = (*ref).substr( coor_value - keys_input[k].key_number * KEY_LENGTH, FRAGMENT_LENGTH); 

					/////////////////////Just For Testing
					char test_char[READ_LENGTH + 1];
					char ref_char[READ_LENGTH + 1];
					strcpy(test_char, fragment.c_str());
					strcpy(ref_char, ref_str.c_str());
					////////////////////Testing END

					ED_result edit_result = editDistanceCal(test_char, ref_char, keys_input[k].key_number);

					// Checking ED_result
					cout << "ref__read: " << ref_char << endl;
					cout << "test_read: " << test_char << endl;
					cout << "key_num__: " << keys_input[k].key_number << endl;
					cout << "correct  : " << edit_result.correct << endl;

					//---------------------------------------------------------------------------------
					if (edit_result.correct) {
						return_result.total_correct_num++;
						(*output_file) << fragment_name << "	"
									<< 0 << "	"
									<< contig_name << "	"
									<< coor_value - keys_input[k].key_number * KEY_LENGTH << "	"
									<< 255 << "	";
						int err_coor = 0;
						for (int err_num = 0; err_num < edit_result.diff_num; err_num++){
							if (edit_result.error[err_num].diff == INSERTION) {
								if(edit_result.error[err_num].location - err_coor > 0) {
									(*output_file) << edit_result.error[err_num].location - err_coor;
									(*output_file) << "M1I";
								}
								err_coor = edit_result.error[err_num].location + 1;
							}
							else if (edit_result.error[err_num].diff == DELETION) {
								if(edit_result.error[err_num].location - err_coor > 0) {
									(*output_file) << edit_result.error[err_num].location - err_coor;
									(*output_file) << "M1D";
								}
								err_coor = edit_result.error[err_num].location + 1;
							}
						}
						if (err_coor < FRAGMENT_LENGTH) {
							(*output_file) << FRAGMENT_LENGTH - err_coor << "M	";
						}
						(*output_file) << "*	0	0	"
									<< test_char << "	"
									<< fragment_qual << "	"
									<< "NM:i:"	<< edit_result.diff_num << "	"
									<< "MD:Z:";
						err_coor = 0;
						for (int err_num = 0; err_num < edit_result.diff_num; err_num++){
							if (edit_result.error[err_num].diff == MISMATCH) {
								if(edit_result.error[err_num].location - err_coor > 0) {
									(*output_file) << edit_result.error[err_num].location - err_coor;
								}
								(*output_file) << edit_result.error[err_num].diff_char;
								err_coor = edit_result.error[err_num].location + 1;
								
							}
							else if (edit_result.error[err_num].diff == DELETION) {
								if(edit_result.error[err_num].location - err_coor > 0) {
									(*output_file) << edit_result.error[err_num].location - err_coor;
								}
								(*output_file) << "^";
								(*output_file) << edit_result.error[err_num].diff_char;
								err_coor = edit_result.error[err_num].location + 1;
							}
						} 
						if (err_coor < FRAGMENT_LENGTH) {
							(*output_file) << FRAGMENT_LENGTH - err_coor;
						}
						(*output_file) << endl;
					//---------------------------------------------------------------------------------

						//cout << "ref_read      : " << ref_str << "  coordinate: "
						//		<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
						//cout << "  result: correct " << endl;
					} else {
						//cout << "ref_read      : " << ref_str << "  coordinate: "
						//		<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
						//cout << "  result: not correct" <<endl;
					}
				}
			}
		}
	}
	return return_result;
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

GPU_data binary_filtering_GPU_cost(string fragment) {
	GPU_data result = {0, 0};
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

	key_struct keys_input[KEY_NUMBER];
	sortPrefilter(keys_input, sort_input);
	previous_result.size = 0;
	final_result return_result;
	return_result.total_binary_search = 0;
	return_result.total_edit_perform = 0;
	return_result.total_correct_num = 0;

	for (int k = 0; k < max_diff_num + 1; k++) {
		for (int i = keys_input[k].key_entry + 1; i <= keys_input[k].key_entry
				+ keys_input[k].key_entry_size; i++) {
			int coor_value = coordinate[i];
			int diff_num = 0;
			if (!searchPrevious(coor_value, keys_input[k].key_number,
					previous_result)) {
				return_result.total_binary_search++;
				for (int j = 0; j < KEY_NUMBER; j++) {
					if (j - diff_num > KEY_NUMBER - max_diff_num)
						break;
					if (!searchKey(coor_value + (keys_input[j].key_number
							- keys_input[k].key_number) * KEY_LENGTH,
							keys_input[j].key_entry,
							keys_input[j].key_entry_size)) {
						diff_num++;
						if (diff_num > max_diff_num)
							break;
					}
				}
				if (diff_num <= max_diff_num) {
					result.ED_count++;
				}
			}
		}
	}

	for (int k = 0; k < max_diff_num + 1; k++) {
		result.AF_count += keys_input[k].key_entry_size;
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
			match_result temp;
			temp.coordinate = coordinate[i];
			temp.relevance = 0;
			temp.key_number = keys_input[k].key_number;
			result.push_back(temp);
		}
	}
	return result;
}

