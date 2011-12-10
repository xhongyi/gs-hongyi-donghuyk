//#define DEBUG
#include <iostream>
#include <iostream>
#include <deque>
#include <list>
#include <map>
#include <cstdio>
#include <fstream>
#include <string>
#include <time.h>
#include "universal_header.h"

#define FRAGMENT_LENGTH 108

using namespace std;

void test_full(string hash_file_name, string ref_file_name, string output_file_name, string result_input_name) {
	set_max_indel_num(3);
	set_max_diff_num(3);
	ifstream ref_file;
	ifstream input_file;
	ofstream store_file;

//	final_result filter_result;
	long long monitor_counter = 0; 		// for operation monitoring
	long long monitor_counter2 = 0; // for operation monitoring

	long long prefilter_time = 0;
	long long memcpy_input_time = 0;
	long long cuda_time = 0;
	long long memcpy_output_time = 0;
	long long analysis_time = 0;
	long long total_time = 0;

	long long acc_prefilter_time = 0;
	long long acc_memcpy_input_time = 0;
	long long acc_cuda_time = 0;
	long long acc_memcpy_output_time = 0;
	long long acc_analysis_time = 0;
	long long acc_total_time = 0;

	int fragment_set  = MAX_FRAGMENT_SET_NUM;
	int fragment_size = MAX_FRAGMENT_SIZE;
	int thread_size   = MAX_THREAD_NUM;
	GPU_fragment test_fragment[fragment_size];
	GPU_fragment* dev_fragment;
	final_result* dev_result;
	final_result *test_result = (final_result*) malloc(sizeof(final_result)*fragment_size);
	cudaMalloc((void**) &dev_fragment, sizeof(GPU_fragment)*fragment_size);
	cudaMalloc((void**) &dev_result,   sizeof(final_result)*fragment_size);

	// get fragment from reference file

	for (int j = 0 ; j < 1 ; j++) {
	//for (int j = 0 ; j < MAX_CONTIG_FILE ; j++) {
		map<int, int> binary_search;
		map<int, int> distribution;
		map<int, int> correct_count;
		char * file_ref	= (char*) malloc(sizeof(char)*50);
		char * file_hash= (char*) malloc(sizeof(char)*50);
		char * file_store= (char*) malloc(sizeof(char)*50);
	   		sprintf(file_ref ,  "%s%i", (char*) ref_file_name.c_str(), j);
	   		sprintf(file_hash,  "%s%i", (char*) hash_file_name.c_str(), j);
	   		sprintf(file_store, "%s%i_%i_%i_%i", (char*) output_file_name.c_str(), j, 
				fragment_set, fragment_size, thread_size);
		cout << "*** ref_file_name  :" << file_ref  << endl;
		cout << "*** hash_file_name :" << file_hash << endl;
		cout << "*** store_file_name:" << file_store << endl;
		// store file 
		store_file.open(file_store);

		// reference file load at string
		string ref;
		cout << "*** Status : Start ref strings" << endl;
		int ref_num = loadRef(file_ref);
		cout << "*** Status : End   ref strings" << endl;

		// hash table load 
		cout << "*** Status : Start load hash table" << endl;
		long long coord_num = loadHash(file_hash);
		cout << "*** Status : End   load hash table" << endl;

		int total_spilled_num = 0;

		ref_file.open(file_ref);
		if (!ref_file.is_open()) {
			cout << " Error File Open : " << file_ref << endl;
			break;
		}
		input_file.open(result_input_name.c_str());
		do {
			time_t start_time;
			time(&start_time);
			// input fragment fetch from result_input
			for (int i = 0 ; i < fragment_size ; i ++ ){
				input_file >> test_fragment[i].fragment;
				if(!input_file.good()){
					fragment_size = i;
					break;
				}
			}
			// Getting the sort key.
			for (int k = 0; k < fragment_size; k++) {
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
//				for (int i = 0; i < KEY_NUMBER; i++) {
//					cout << " fragment_set: "   << k
//						 << " key_number:"	  << test_fragment[k].sorted_keys[i].key_number
//						 << " key_entry: "	  << test_fragment[k].sorted_keys[i].key_entry
//						 << " key_entry_size: " << test_fragment[k].sorted_keys[i].key_entry_size
//						 << " base: "		   << test_fragment[k].sorted_keys[i].base << endl;
//				}
//				cout << "****************************************************************************" << endl;
			}
			time_t start_memcpy_input_time;
			time(&start_memcpy_input_time);
			cudaMemcpy(dev_fragment, &test_fragment, sizeof(GPU_fragment)*fragment_size, cudaMemcpyHostToDevice);
			time_t start_cuda_time;
			time(&start_cuda_time);
			searchFragment <<<fragment_set, thread_size>>> (dev_fragment, fragment_size, 
						dev_ref_string, dev_hash_table, dev_coordinate, 3, 3, dev_result);
			time_t start_memcpy_output_time;
			time(&start_memcpy_output_time);
			cudaMemcpy(test_result, dev_result, sizeof(final_result)*fragment_size, cudaMemcpyDeviceToHost);
			time_t start_analysis_time;
			time(&start_analysis_time);
			for (int j = 0; j < fragment_size; j++) {
//				cout << "****************************************************************************" << endl;
//				cout << "Spilled	  : " << test_result[j].spilled << endl;
//				cout << "Result Size  : " << test_result[j].size << endl;
//				cout << "****************************************************************************" << endl;
//				cout << "Fragment	 : " << test_result[j].fragment << endl;
	
//				cout << "Fragment	 : ";
//				for (int i = 0; i < READ_LENGTH; i++){
//					cout << test_result[j].fragment[i];
//				}
//				cout << endl << "* Size  : " << test_result[j].size << endl;
				int tmp_size = test_result[j].size;
				if(tmp_size > 100) {
					tmp_size = 100;
				}
				if(test_result[j].spilled == true) {
					total_spilled_num = total_spilled_num + 1;
				}
//				for (int i = 0; i < tmp_size; i++) {
//					cout << "* Result coor  : " << test_result[j].coor_results[i].coordiante;
//					cout << "   Result diff  : " << test_result[j].coor_results[i].diff_num << endl;
//				}
//				cout << "****************************************************************************" << endl;
	            correct_count[tmp_size]++;
			}
			monitor_counter = monitor_counter + fragment_size;
			monitor_counter2 = monitor_counter2 + fragment_size;
			if (monitor_counter >= 10000) {
				fprintf(stdout, "hash distribution count: %lld \n", monitor_counter2);
					monitor_counter = 0;
				}
			if (monitor_counter2 >= 1000000) {
					monitor_counter2 = 0;
			}
			time_t end_time;
			time(&end_time);
 			prefilter_time 		= prefilter_time + difftime(start_memcpy_input_time, start_time);
			memcpy_input_time 	= memcpy_input_time + difftime(start_cuda_time, start_memcpy_input_time);
 			cuda_time 			= cuda_time + difftime(start_memcpy_output_time, start_cuda_time);
			memcpy_output_time 	= memcpy_output_time + difftime(start_analysis_time, start_memcpy_output_time);
 			analysis_time 		= analysis_time + difftime(end_time, start_analysis_time);
 			total_time 			= total_time + difftime(end_time, start_time);
		}while(input_file.good());
		fragment_size = MAX_FRAGMENT_SIZE;
		cudaFree(dev_fragment);
		cudaFree(dev_result);
		freeHash();
		freeRef();
	
 		acc_prefilter_time 		= acc_prefilter_time + prefilter_time;
		acc_memcpy_input_time 	= acc_memcpy_input_time + memcpy_input_time;
 		acc_cuda_time 			= acc_cuda_time + cuda_time;
		acc_memcpy_output_time 	= acc_memcpy_output_time + memcpy_output_time;
 		acc_analysis_time 		= acc_analysis_time + analysis_time;
		acc_total_time 			= acc_total_time + total_time;
		ref_file.close();
		input_file.close();
	
		long long total_fragment_num2 = 0;
		long long total_pass_num = 0;
		store_file << endl << "Number of Edit-distance Passed" << endl;
		for (map<int, int>::iterator p = correct_count.begin(); p != correct_count.end(); p++) {
			store_file << "index :" << p->first << "	num :" << p->second << endl;
			total_fragment_num2 = total_fragment_num2 + p->second;
			total_pass_num = total_pass_num + p->first * p->second;
		}
		store_file << endl;
		store_file << "---------------------------------------------" << endl;
		store_file << "total_fragment_num : " << total_fragment_num2 << endl;
		store_file << "total_pass_num____ : " << total_pass_num << endl;
		store_file << "total_spilled_num_ : " << total_spilled_num << endl;
		store_file << "Acc_prefilter_time : " << acc_prefilter_time << endl;
		store_file << "Acc_memcpy_input__ : " << acc_memcpy_input_time << endl;
		store_file << "Acc_cuda__________ : " << acc_cuda_time << endl;
		store_file << "Acc_memcpy_output_ : " << acc_memcpy_output_time << endl;
		store_file << "Acc_analysis_time_ : " << acc_analysis_time << endl;
		store_file << "Accumulated Time__ : " << acc_total_time << endl;
		store_file << "---------------------------------------------" << endl;
	
		cout << "---------------------------------------------" << endl;
		cout << "total_fragment_num : " << total_fragment_num2 << endl;
		cout << "total_pass_num____ : " << total_pass_num << endl;
		cout << "total_spilled_num_ : " << total_spilled_num << endl;
		cout << "Acc_prefilter_time : " << acc_prefilter_time << endl;
		cout << "Acc_memcpy_input__ : " << acc_memcpy_input_time << endl;
		cout << "Acc_cuda__________ : " << acc_cuda_time << endl;
		cout << "Acc_memcpy_output_ : " << acc_memcpy_output_time << endl;
		cout << "Acc_analysis_time_ : " << acc_analysis_time << endl;
		cout << "Accumulated Time__ : " << acc_total_time << endl;
		cout << "---------------------------------------------" << endl;
		store_file.close();
	}
	cout << "Accumulated Time : " << acc_total_time << endl;
}

int main() {
	char * file_ref		= (char*) malloc(sizeof(char)*50);	
	char * file_input	= (char*) malloc(sizeof(char)*50);	
	char * file_hash	= (char*) malloc(sizeof(char)*50);
	char * file_dist	= (char*) malloc(sizeof(char)*50);
	sprintf(file_ref,  "%s", "result_ref_");
	sprintf(file_hash, "%s", "result_hash_");
	sprintf(file_input, "%s", "result_input");

	fprintf(stdout,"Mode : Cheapest Select\n");
	sprintf(file_dist, "%s", "result_dist_cheap_");
	test_full(file_hash, file_ref, file_dist, file_input);

	free(file_ref);
	free(file_input);
	free(file_hash);
	free(file_dist);
	return 0;
}

			time_t start_time;
			time_t start_memcpy_input_time;
			time_t start_cuda_time;
			time_t start_memcpy_output_time;
			time_t start_analysis_time;
			time_t end_time;

