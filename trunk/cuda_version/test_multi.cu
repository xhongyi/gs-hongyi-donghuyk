/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *	  Author: mac
 */
#include <iostream>
#include "universal_header.h"

using namespace std;


#include "hash_test.cu"


int test_cuda(void) {

	int ref_num = loadRef("toy_ref_0");
	long long coord_num = loadHash("toy_hash_0");

	hash_test(coord_num);
	ref_test(ref_num);
	int fragment_set = 4;
	GPU_fragment test_fragment[fragment_set];
	strcpy(test_fragment[0].fragment, 
	"GGGTGGTAGGTGCAGAGACGGGAGGGGCAGAGCCGCAGGCACAGCCAAGAGGGCTGAAGAAATGGTAGAACGGAGCAGCTGGTGATGTGTGGGCCCACCGGCCCCAGG");
	strcpy(test_fragment[1].fragment, 
	"TGGCCCTGGGAGAGCAGGTGGAAGATCAGGCAGGCCATCGCTGCCACAGAACCCAGTGGATTGGCCTAGGTGGGATCTCTGAGCTCAACAAGCCCTCTCTCTTAGTTT");
	strcpy(test_fragment[2].fragment, 
	"CCTAACCCAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCCTAACCCTAACCCTAACCCTAACCCTAACCTAACCCTAACCCTAACCCTAACCCTAACC");
	strcpy(test_fragment[3].fragment, 
	"CCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCCTAACCCTAACCCTAACCCTAACCCTCGCGGTACCCTCAGCCGGCCCGCCCGCCCGGGTCTGACCTGAGG");

	//Getting the sort key.
	cout << "********** Fragment Input **************************************************" << endl;
	for (int k = 0; k < fragment_set; k++) {
		key_struct sort_input[KEY_NUMBER];
		for (int i = 0; i < KEY_NUMBER; i++) {
			char key[KEY_LENGTH];
			for (int j = 0; j < KEY_LENGTH; j++) {
				key[j] = test_fragment[k].fragment[j + KEY_LENGTH * i];
			}
			int key_hash = hashVal(key);
			int key_entry = hash_table[key_hash];
			int key_entry_size = coordinate[key_entry];
			sort_input[i].order = 0;
			sort_input[i].key_number = i;
			sort_input[i].key_entry = key_entry;
			sort_input[i].key_entry_size = key_entry_size;
		}
		sortPrefilter(test_fragment[k].sorted_keys, sort_input);
		for (int i = 0; i < KEY_NUMBER; i++) {
			cout << " fragment_set: "	<< k
				 << " key_number:"		<< test_fragment[k].sorted_keys[i].key_number 
				 << " key_entry: "		<< test_fragment[k].sorted_keys[i].key_entry 
				 << " key_entry_size: " << test_fragment[k].sorted_keys[i].key_entry_size 
				 << " base: " 			<< test_fragment[k].sorted_keys[i].base << endl;
		}
		cout << "****************************************************************************" << endl;
	}

	GPU_fragment* dev_fragment;
	final_result* dev_result;
	final_result test_result[fragment_set];

	cudaMalloc((void**) &dev_fragment, sizeof(GPU_fragment)*fragment_set);
	cudaMalloc((void**) &dev_result,   sizeof(final_result)*fragment_set);
	cudaMemcpy(dev_fragment, &test_fragment, sizeof(GPU_fragment)*fragment_set, cudaMemcpyHostToDevice);
	searchFragment <<<fragment_set, 1>>> (dev_fragment, fragment_set, dev_ref_string, dev_hash_table, 
								dev_coordinate, 5, 5, dev_result);
	cudaMemcpy(test_result, dev_result, sizeof(final_result)*fragment_set, cudaMemcpyDeviceToHost);
	for (int j = 0; j < fragment_set; j++) {
		cout << "****************************************************************************" << endl;
		cout << "Fragment	  : " << test_result[j].fragment << endl;
		cout << "Spilled	  : " << test_result[j].spilled << endl;
		cout << "Result Size  : " << test_result[j].size << endl;
		cout << "****************************************************************************" << endl;
		for (int i = 0; i < test_result[j].size; i++) {
			cout << "* Result coor  : " << test_result[j].coor_results[i].coordiante;
			cout << "	Result diff  : " << test_result[j].coor_results[i].diff_num << endl;
		}
		cout << "****************************************************************************" << endl;
	}
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
