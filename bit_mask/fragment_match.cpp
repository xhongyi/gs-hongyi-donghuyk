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
//int mask_range;
//int bit_mask[INDEX_NUM];
previous_coor previous_result;
bitmask bm;

void getHashTablePtr(int ** ptr) {
	*ptr = hash_table;
}

void getCoordinatePtr(int ** ptr) {
	*ptr = coordinate;
}

void loadHash(string hash_name, int hash_file_num) {
	hashReconstructor(&hash_table, &coordinate, hash_name.c_str());
	if (bit_mask_on)
		bm.load_mask(hash_file_num);
}

void freeHash() {
	free(hash_table);
	free(coordinate);
	if (bit_mask_on)
		bm.free_mask();
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
/*
bool test_mask(int target_coor, int hash_val) {
	int lower_bound = target_coor - max_indel_num;
	int upper_bound = target_coor + max_indel_num;
	if (lower_bound < 0)
		lower_bound = 0;
	bool lower_cover = (1 << (target_coor - max_indel_num) / mask_range)
			&& bit_mask[hash_val];
	bool upper_cover = (1 >> (target_coor + max_indel_num) / mask_range)
			&& bit_mask[hash_val];
	if (lower_cover || upper_cover)
		return true;
	else
		return false;
}
*/
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
		for (int i = 0; i < key_number_; i++) {
			sort_result[i].key_number = sort_input[i].key_number;
			sort_result[i].key_entry = sort_input[i].key_entry;
			sort_result[i].key_entry_size = sort_input[i].key_entry_size;
			sort_result[i].hash_val = sort_input[i].hash_val;
		}
		return true;
	}
	for (int i = 0; i < key_number_; i++) {
		for (int j = 0; j < key_number_; j++) {
			if (sort_input[i].key_entry_size > sort_input[j].key_entry_size) {
				sort_input[i].order = sort_input[i].order + 1;
			}
		}
	}
	int loop_index = 0;
	for (int i = 0; i < key_number_; i++) {
		for (int j = 0; j < key_number_; j++) {
			if (sort_input[j].order == i) {
				sort_result[loop_index].key_entry = sort_input[j].key_entry;
				sort_result[loop_index].key_number = sort_input[j].key_number;
				sort_result[loop_index].key_entry_size
						= sort_input[j].key_entry_size;
				sort_result[loop_index].hash_val = sort_input[j].hash_val;
				loop_index = loop_index + 1;
			}
			//if (loop_index == max_diff_num + 1)
			//	return true;
		}
	}
	return true;//false;
}

bool sortPrefilterAllowN(key_struct* sort_result, key_struct* sort_input,
		int available_key_num) {
	if (select_cheapest == false) {
		for (int i = 0; i < available_key_num; i++) {
			sort_result[i].key_number = sort_input[i].key_number;
			sort_result[i].key_entry = sort_input[i].key_entry;
			sort_result[i].key_entry_size = sort_input[i].key_entry_size;
			sort_result[i].hash_val = sort_input[i].hash_val;
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
				sort_result[loop_index].key_entry_size
						= sort_input[j].key_entry_size;
				sort_result[loop_index].hash_val = sort_input[j].hash_val;
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
	key_struct* sort_input = (key_struct*) malloc(
			key_number_ * sizeof(key_struct));
	key_struct* keys_input = (key_struct*) malloc(
			key_number_ * sizeof(key_struct));
	for (int i = 0; i < key_number_; i++) {
		string key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
		int key_hash = hashVal(key);
		int key_entry = hash_table[key_hash];
		int key_entry_size = coordinate[key_entry];
		sort_input[i].order = 0;
		sort_input[i].key_number = i;
		sort_input[i].key_entry = key_entry;
		sort_input[i].key_entry_size = key_entry_size;
		sort_input[i].hash_val = key_hash;
	}

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
				for (int j = 0; j < key_number_; j++) {
					if (j - diff_num > key_number_ - max_diff_num)
						break;
					bool pass;
					if (!bit_mask_on)
						pass = searchKey(
								coor_value + (keys_input[j].key_number
										- keys_input[k].key_number)
										* KEY_LENGTH, keys_input[j].key_entry,
								keys_input[j].key_entry_size);
					else {
						if (bm.test_mask(
								coor_value + (keys_input[j].key_number
										- keys_input[k].key_number)
										* KEY_LENGTH, keys_input[j].hash_val) )
							pass = searchKey(
									coor_value + (keys_input[j].key_number
											- keys_input[k].key_number)
											* KEY_LENGTH, keys_input[j].key_entry,
									keys_input[j].key_entry_size);
						else
							pass = false;
					}
					if (!pass) {
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
					string ref_str(fragment_length_, 'A');
					ref_str = (*ref).substr(
							coor_value - keys_input[k].key_number * KEY_LENGTH,
							fragment_length_); //start_coor;

					/////////////////////Just For Testing
					char* test_char = (char*) malloc(
							(fragment_length_ + 1) * sizeof(char));
					char* ref_char = (char*) malloc(
							(fragment_length_ + 1) * sizeof(char));
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
						//cout << "ref_read      : " << ref_str << "  coordinate: "
						//		<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
						//cout << "  result: correct " << endl;
					} else {
						//cout << "ref_read      : " << ref_str << "  coordinate: "
						// 		<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
						//cout << "  result: not correct" <<endl;
					}
					free(test_char);
					free(ref_char);
				}
			}
		}
	}
	free(sort_input);
	free(keys_input);
	return return_result;
}

final_result searchFragment_fastq(string fragment, string* ref,
		ofstream * output_file, char* contig_name, string fragment_name,
		string fragment_qual) {
	key_struct* sort_input = (key_struct*) malloc(
			key_number_ * sizeof(key_struct));
	key_struct* keys_input = (key_struct*) malloc(
			key_number_ * sizeof(key_struct));
	int available_key_num = 0;
	for (int i = 0; i < key_number_; i++) {
		string key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
		int key_hash = hashVal(key);
		if (key_hash >= 0) { // check if there is 'N'
			int key_entry = hash_table[key_hash];
			int key_entry_size = coordinate[key_entry];
			sort_input[available_key_num].order = 0;
			sort_input[available_key_num].key_number = i;
			sort_input[available_key_num].key_entry = key_entry;
			sort_input[available_key_num].key_entry_size = key_entry_size;
			sort_input[available_key_num].hash_val = key_hash;
			available_key_num++;
		}
	}
	int n_num = key_number_ - available_key_num;
	/*
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
	 */
	previous_result.size = 0;
	final_result return_result;
	return_result.total_bit_mask_success = 0;
	return_result.total_binary_success = 0;
	return_result.total_filtering = 0;
	return_result.total_binary_search = 0;
	return_result.total_edit_perform = 0;
	return_result.total_correct_num = 0;

	if (n_num > max_diff_num) {
		return return_result;
	}
	int potential_diff_num = max_diff_num - n_num;

	sortPrefilterAllowN(keys_input, sort_input, available_key_num); // DHL
	/*
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
	 // key_number_ -> available_key_num
	 // max_diff_num -> potential_diff_num
	 // ----------------------------------
	 */
	int operating_key_num = max_diff_num + 1 - n_num;
	if (available_key_num < max_diff_num + 1 - n_num) {
		operating_key_num = available_key_num;
	}
	/*
	 // Checking operating key with N
	 cout << "operating key: " << operating_key_num << endl;
	 cout << "fragment_leng: " << fragment_length_ << endl;
	 cout << "key_number   : " << key_number_ << endl;
	 */
	for (int k = 0; k < operating_key_num; k++) {
		for (int i = keys_input[k].key_entry + 1; i <= keys_input[k].key_entry
				+ keys_input[k].key_entry_size; i++) {
			int coor_value = coordinate[i];
			int diff_num = 0;
			if (!searchPrevious(coor_value, keys_input[k].key_number,
					previous_result)) {
				return_result.total_filtering++;

				//-------------Binary Search---------------------------------------------------
				for (int j = 0; j < available_key_num; j++) {
					if (j - diff_num - n_num > key_number_ - max_diff_num)
						break;
					bool pass;
					if (keys_input[j].key_entry_size < 100 || !bit_mask_on) {
						pass = searchKey(
								coor_value + (keys_input[j].key_number
										- keys_input[k].key_number)
										* KEY_LENGTH, keys_input[j].key_entry,
								keys_input[j].key_entry_size);
						return_result.total_binary_search++;
					}
					else {
						if (/*!bm.over_fifth(keys_input[j].hash_val) || */bm.test_mask(
								coor_value + (keys_input[j].key_number
										- keys_input[k].key_number)
										* KEY_LENGTH, keys_input[j].hash_val) ) {
							pass = searchKey(
									coor_value + (keys_input[j].key_number
											- keys_input[k].key_number)
											* KEY_LENGTH, keys_input[j].key_entry,
									keys_input[j].key_entry_size);
							return_result.total_binary_search++;
							if (!pass)
								bm.record_fail(keys_input[j].hash_val);
						}
						else {
							//cout << "asdfasdfasdf" << endl;
							pass = false;
							return_result.total_bit_mask_success++;
						}
					}
					if (!pass) {
						diff_num++;
						if (diff_num + n_num > max_diff_num)
							break;
					}
					else
						return_result.total_binary_success++;
				}
				//-----------------------------------------------------------------------------

				if (diff_num + n_num <= max_diff_num) {
					if (previous_result.size <= PREFILTER_SIZE) {
						previous_result.coor[previous_result.size] = coor_value
								- keys_input[k].key_number * KEY_LENGTH; //start_coor;
						previous_result.size++;
					}
					return_result.total_edit_perform++;
					string ref_str(fragment_length_, 'A');
					/*
					 cout << "--------------------------------" << endl;
					 cout << "Target coor     :" << coor_value - keys_input[k].key_number * KEY_LENGTH << endl;
					 cout << "coor_value      :" << coor_value <<endl;
					 cout << "k				 :" << k << endl;
					 cout << "key_number 	 :" << keys_input[k].key_number << endl;
					 cout << "KEY_LENGTH 	 :" << KEY_LENGTH << endl;
					 cout << "--------------------------------" << endl;
					 cout << "fragment_length_:" << fragment_length_ << endl;
					 */
					if (coor_value - keys_input[k].key_number * KEY_LENGTH < 0) {
						int tmp_num = coor_value - keys_input[k].key_number
								* KEY_LENGTH;
						ref_str.clear();
						for (int n = tmp_num; n < 0; n++) {
							ref_str += "N";
						}
						ref_str += (*ref).substr(0, fragment_length_ + tmp_num);
					} else {
						ref_str = (*ref).substr(
								coor_value - keys_input[k].key_number
										* KEY_LENGTH, fragment_length_);
					}
					/////////////////////Just For Testing
					char* test_char = (char*) malloc(
							(fragment_length_ + 1) * sizeof(char));
					char* ref_char = (char*) malloc(
							(fragment_length_ + 1) * sizeof(char));
					strcpy(test_char, fragment.c_str());
					strcpy(ref_char, ref_str.c_str());
					////////////////////Testing END

					ED_result edit_result = editDistanceCal(test_char,
							ref_char, keys_input[k].key_number);
					/*
					 // Checking ED_result
					 cout << "test_read: " << test_char << endl;
					 cout << "ref__read: " << ref_char << endl;
					 cout << "key_num__: " << keys_input[k].key_number << endl;
					 cout << "correct  : " << edit_result.correct << endl;
					 */

					if (edit_result.correct)
						return_result.total_correct_num++;

					//---------------------------------------------------------------------------------
					if (edit_result.correct && print_detail) {
						//(*output_file) << "test_read: " << test_char << endl;
						//(*output_file) << "ref__read: " << ref_char << endl;
						(*output_file) << fragment_name << "	";
						if (reverse_mode == false) {
							(*output_file) << 0 << "	";
						} else {
							(*output_file) << 16 << "	";
						}
						(*output_file) << contig_name << "	";
						(*output_file) << coor_value - keys_input[k].key_number
								* KEY_LENGTH + 1 << "	";
						(*output_file) << 255 << "	";
						int err_coor = 0;
						for (int err_num = 0; err_num < edit_result.diff_num; err_num++) {
							if (edit_result.error[err_num].diff == INSERTION) {
								if (edit_result.error[err_num].location
										- err_coor > 0) {
									(*output_file)
											<< edit_result.error[err_num].location
													- err_coor;
									(*output_file) << "M1I";
								}
								err_coor = edit_result.error[err_num].location
										+ 1;
							} else if (edit_result.error[err_num].diff
									== DELETION) {
								if (edit_result.error[err_num].location
										- err_coor > 0) {
									(*output_file)
											<< edit_result.error[err_num].location
													- err_coor;
									(*output_file) << "M1D";
								}
								err_coor = edit_result.error[err_num].location
										+ 1;
							}
						}
						if (err_coor < fragment_length_) {
							(*output_file) << fragment_length_ - err_coor
									<< "M	";
						}
						(*output_file) << "*	0	0	" << test_char << "	"
								<< fragment_qual << "	" << "NM:i:"
								<< edit_result.diff_num << "	" << "MD:Z:";
						err_coor = 0;
						for (int err_num = 0; err_num < edit_result.diff_num; err_num++) {
							if (edit_result.error[err_num].diff == MISMATCH) {
								if (edit_result.error[err_num].location
										- err_coor > 0) {
									(*output_file)
											<< edit_result.error[err_num].location
													- err_coor;
								}
								(*output_file)
										<< edit_result.error[err_num].diff_char;
								err_coor = edit_result.error[err_num].location
										+ 1;

							} else if (edit_result.error[err_num].diff
									== DELETION) {
								if (edit_result.error[err_num].location
										- err_coor > 0) {
									(*output_file)
											<< edit_result.error[err_num].location
													- err_coor;
								}
								(*output_file) << "^";
								(*output_file)
										<< edit_result.error[err_num].diff_char;
								err_coor = edit_result.error[err_num].location
										+ 1;
							}
						}
						if (err_coor < fragment_length_) {
							(*output_file) << fragment_length_ - err_coor;
						}
						(*output_file) << endl;
						//---------------------------------------------------------------------------------
						//for (int err_num = 0; err_num < edit_result.diff_num; err_num++){
						//(*output_file) << "DEBUG: " << edit_result.error[err_num].diff << " "
						//							<< edit_result.error[err_num].location << " "
						//							<< edit_result.error[err_num].diff_char << endl;
						//}

						//cout << "ref_read      : " << ref_str << "  coordinate: "
						//		<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
						//cout << "  result: correct " << endl;
					} else {
						//cout << "ref_read      : " << ref_str << "  coordinate: "
						//		<< (*it_result).coordinate << "  Key_number: "<< (*it_result).key_number;
						//cout << "  result: not correct" <<endl;
					}
					free(test_char);
					free(ref_char);
				}
			}
		}
	}
	free(sort_input);
	free(keys_input);
	return return_result;
}

int binary_filtering_cost(string fragment) {
	int result = 0;
	int status = 0;
	key_struct* sort_input = (key_struct*) malloc(
			key_number_ * sizeof(key_struct));
	key_struct* keys_input = (key_struct*) malloc(
			(max_diff_num + 1) * sizeof(key_struct));
	for (int i = 0; i < key_number_; i++) {
		string key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
		int key_hash = hashVal(key);
		int key_entry = hash_table[key_hash];
		int key_entry_size = coordinate[key_entry];
		sort_input[i].order = 0;
		sort_input[i].key_number = i;
		sort_input[i].key_entry = key_entry;
		sort_input[i].key_entry_size = key_entry_size;
	}

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
	free(sort_input);
	free(keys_input);
	return result;
}

GPU_data binary_filtering_GPU_cost(string fragment) {
	GPU_data result = { 0, 0 };
	key_struct* sort_input = (key_struct*) malloc(
			key_number_ * sizeof(key_struct));
	key_struct* keys_input = (key_struct*) malloc(
			key_number_ * sizeof(key_struct));
	for (int i = 0; i < key_number_; i++) {
		string key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
		int key_hash = hashVal(key);
		int key_entry = hash_table[key_hash];
		int key_entry_size = coordinate[key_entry];
		sort_input[i].order = 0;
		sort_input[i].key_number = i;
		sort_input[i].key_entry = key_entry;
		sort_input[i].key_entry_size = key_entry_size;
	}

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
				for (int j = 0; j < key_number_; j++) {
					if (j - diff_num > key_number_ - max_diff_num)
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
					result.ED_count++;
				}
			}
		}
	}

	for (int k = 0; k < max_diff_num + 1; k++) {
		result.AF_count += keys_input[k].key_entry_size;
	}
	free(sort_input);
	free(keys_input);
	return result;
}

list<match_result> noFilterSearch(string fragment) {
	list<match_result> result;
	key_struct* sort_input = (key_struct*) malloc(
			key_number_ * sizeof(key_struct));
	key_struct* keys_input = (key_struct*) malloc(
			(max_diff_num + 1) * sizeof(key_struct));
	for (int i = 0; i < key_number_; i++) {
		string key = fragment.substr(KEY_LENGTH * i, KEY_LENGTH);
		int key_hash = hashVal(key);
		int key_entry = hash_table[key_hash];
		int key_entry_size = coordinate[key_entry];
		sort_input[i].order = 0;
		sort_input[i].key_number = i;
		sort_input[i].key_entry = key_entry;
		sort_input[i].key_entry_size = key_entry_size;
	}

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
	free(sort_input);
	free(keys_input);
	return result;
}

