/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *	  Author: mac
 */
#include <iostream>
#include "universal_header.h"

using namespace std;

int test_cuda(void) {

	long long coord_num = loadHash("toy_hash_0");
    long long ref_num = loadRef("toy_ref_0");

	GPU_fragment test_fragment;
	strcpy(
			test_fragment.fragment,
			"GGGTGGTAGGTGCAGAGACGGGAGGGGCAGAGCCGCAGGCACAGCCAAGAGGGCTGAAGAAATGGTAGAACGGAGCAGCTGGTGATGTGTGGGCCCACCGGCCCCAGG");
	//Getting the sort key.
	key_struct sort_input[KEY_NUMBER];
	for (int i = 0; i < KEY_NUMBER; i++) {
		char key[KEY_LENGTH];
		for (int j = 0; j < KEY_LENGTH; j++) {
			key[j] = test_fragment.fragment[j + KEY_LENGTH * i];
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

	sortPrefilter(test_fragment.sorted_keys, sort_input);
	for (int i = 0; i < KEY_NUMBER; i++) {
		cout << "key_number:" << test_fragment.sorted_keys[i].key_number << " key_entry: " << test_fragment.sorted_keys[i].key_entry << " key_entry_size: " << test_fragment.sorted_keys[i].key_entry_size << " base: " << test_fragment.sorted_keys[i].base << endl;
	}

	cout << "Search Fragment: " << test_fragment.fragment << endl;

	GPU_fragment* dev_fragment;
	final_result* dev_result;
	final_result* test_result = (final_result*) malloc(sizeof(final_result));

	cudaMalloc((void**) &dev_fragment, sizeof(GPU_fragment));
	cudaMalloc((void**) &dev_result, sizeof(final_result));
	cudaMemcpy(dev_fragment, &test_fragment, sizeof(GPU_fragment),
			cudaMemcpyHostToDevice);

	searchFragment <<<1, 2>>> (dev_fragment, 1, dev_ref_string, dev_hash_table, dev_coordinate, 5, 5, dev_result);

	cudaMemcpy(test_result, dev_result, sizeof(final_result),
			cudaMemcpyDeviceToHost);
	cout << "Fragment	 : " << (*test_result).fragment << endl;
	cout << "Spilled	  : " << (*test_result).spilled << endl;
	cout << "Result Size  : " << (*test_result).size << endl;
	for (int i = 0; i < (*test_result).size; i++) {
		cout << "Result coor  : " << (*test_result).coor_results[i].coordiante
				<< endl;
		cout << "Result diff  : " << (*test_result).coor_results[i].diff_num
				<< endl;
	}

	free(test_result);
	cudaFree(dev_fragment);
	cudaFree(dev_result);
	freeHash();
	freeRef();
	return 0;
}

int main() {
	test_cuda();
	return 0;
}
