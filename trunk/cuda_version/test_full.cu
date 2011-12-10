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
//	ofstream store_file;
    FILE * pFileW;

//	final_result filter_result;
	long long monitor_counter = 0; 		// for operation monitoring
	long long monitor_counter2 = 0; // for operation monitoring

	float prefilter_time = 0;
	float memcpy_input_time = 0;
	float cuda_time = 0;
	float memcpy_output_time = 0;
	float analysis_time = 0;
	float total_time = 0;

	float acc_prefilter_time = 0;
	float acc_memcpy_input_time = 0;
	float acc_cuda_time = 0;
	float acc_memcpy_output_time = 0;
	float acc_analysis_time = 0;
	float acc_total_time = 0;

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
//		store_file.open(file_store);
		pFileW = fopen(file_store, "w");

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
			cudaEvent_t start_prefilter_time, stop_prefilter_time;
			cudaEventCreate(&start_prefilter_time);
			cudaEventRecord(start_prefilter_time, 0);
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
			cudaEventCreate(&stop_prefilter_time);
			cudaEventRecord(stop_prefilter_time, 0);
			cudaEventSynchronize(stop_prefilter_time);
			float tmp_prefilter_time;
			cudaEventElapsedTime(&tmp_prefilter_time, start_prefilter_time, stop_prefilter_time);
 			prefilter_time = prefilter_time + tmp_prefilter_time;
			cudaEventDestroy(start_prefilter_time);
			cudaEventDestroy(stop_prefilter_time);

			cudaEvent_t start_memcpy_input_time, stop_memcpy_input_time;
			cudaEventCreate(&start_memcpy_input_time);
			cudaEventRecord(start_memcpy_input_time,0);
			cudaMemcpy(dev_fragment, &test_fragment, sizeof(GPU_fragment)*fragment_size, cudaMemcpyHostToDevice);
			cudaEventCreate(&stop_memcpy_input_time);
			cudaEventRecord(stop_memcpy_input_time, 0);
			cudaEventSynchronize(stop_memcpy_input_time);
			float tmp_memcpy_input_time;
			cudaEventElapsedTime(&tmp_memcpy_input_time, start_memcpy_input_time, stop_memcpy_input_time);
			memcpy_input_time = memcpy_input_time + tmp_memcpy_input_time;
			cudaEventDestroy(start_memcpy_input_time);
			cudaEventDestroy(stop_memcpy_input_time);

			cudaEvent_t start_cuda_time, stop_cuda_time;
			cudaEventCreate(&start_cuda_time);
			cudaEventRecord(start_cuda_time,0);
			searchFragment <<<fragment_set, thread_size>>> (dev_fragment, fragment_size, 
						dev_ref_string, dev_hash_table, dev_coordinate, 3, 3, dev_result);
			cudaEventCreate(&stop_cuda_time);
			cudaEventRecord(stop_cuda_time, 0);
			cudaEventSynchronize(stop_cuda_time);
			float tmp_cuda_time;
			cudaEventElapsedTime(&tmp_cuda_time, start_cuda_time, stop_cuda_time);
			cuda_time = cuda_time + tmp_cuda_time;
			cudaEventDestroy(start_cuda_time);
			cudaEventDestroy(stop_cuda_time);

			cudaEvent_t start_memcpy_output_time, stop_memcpy_output_time;
			cudaEventCreate(&start_memcpy_output_time);
			cudaEventRecord(start_memcpy_output_time,0);
			cudaMemcpy(test_result, dev_result, sizeof(final_result)*fragment_size, cudaMemcpyDeviceToHost);
			cudaEventCreate(&stop_memcpy_output_time);
			cudaEventRecord(stop_memcpy_output_time, 0);
			cudaEventSynchronize(stop_memcpy_output_time);
			float tmp_memcpy_output_time;
			cudaEventElapsedTime(&tmp_memcpy_output_time, start_memcpy_output_time, stop_memcpy_output_time);
			memcpy_output_time = memcpy_output_time + tmp_memcpy_output_time;
			cudaEventDestroy(start_memcpy_output_time);
			cudaEventDestroy(stop_memcpy_output_time);

			cudaEvent_t start_analysis_time, stop_analysis_time;
			cudaEventCreate(&start_analysis_time);
			cudaEventRecord(start_analysis_time,0);
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
			cudaEventCreate(&stop_analysis_time);
			cudaEventRecord(stop_analysis_time, 0);
			cudaEventSynchronize(stop_analysis_time);
			float tmp_analysis_time;
			cudaEventElapsedTime(&tmp_analysis_time, start_analysis_time, stop_analysis_time);
			analysis_time = analysis_time + tmp_analysis_time;
			cudaEventDestroy(start_analysis_time);
			cudaEventDestroy(stop_analysis_time);
			



		}while(input_file.good());
		fragment_size = MAX_FRAGMENT_SIZE;
		cudaFree(dev_fragment);
		cudaFree(dev_result);
		freeHash();
		freeRef();
 		total_time = total_time + prefilter_time + memcpy_input_time + cuda_time 
				+ memcpy_output_time + analysis_time;
	
 		acc_prefilter_time 		= acc_prefilter_time 	+ prefilter_time;
		acc_memcpy_input_time 	= acc_memcpy_input_time + memcpy_input_time;
 		acc_cuda_time 			= acc_cuda_time 		+ cuda_time;
		acc_memcpy_output_time 	= acc_memcpy_output_time + memcpy_output_time;
 		acc_analysis_time 		= acc_analysis_time + analysis_time;
		acc_total_time 			= acc_total_time + total_time;
		ref_file.close();
		input_file.close();
	
		long long total_fragment_num2 = 0;
		long long total_pass_num = 0;
		fprintf(pFileW,"Number of Edit-distance Passed\n");
		for (map<int, int>::iterator p = correct_count.begin(); p != correct_count.end(); p++) {
			fprintf(pFileW,"index : %i	num : %i\n", p->first, p->second);
			total_fragment_num2 = total_fragment_num2 + p->second;
			total_pass_num = total_pass_num + p->first * p->second;
		}
		fprintf(pFileW,"\n");
	
		fprintf(pFileW,"---------------------------------------------\n");
		fprintf(pFileW,"total_fragment_num : %lliea\n", total_fragment_num2);
		fprintf(pFileW,"total_pass_num____ : %lliea\n", total_pass_num);
		fprintf(pFileW,"total_spilled_num_ : %iea\n", total_spilled_num);
		fprintf(pFileW,"---------------------------------------------\n");
		fprintf(pFileW,"Acc_prefilter_time : %3.1fms\n", acc_prefilter_time);
		fprintf(pFileW,"Acc_memcpy_input__ : %3.1fms\n", acc_memcpy_input_time);
		fprintf(pFileW,"Acc_cuda__________ : %3.1fms\n", acc_cuda_time);
		fprintf(pFileW,"Acc_memcpy_output_ : %3.1fms\n", acc_memcpy_output_time);
		fprintf(pFileW,"Acc_analysis_time_ : %3.1fms\n", acc_analysis_time);
		fprintf(pFileW,"Accumulated Time__ : %3.1fms\n", acc_total_time);
		fprintf(pFileW,"---------------------------------------------\n");

		printf("---------------------------------------------\n");
		printf("total_fragment_num : %lliea\n", total_fragment_num2);
		printf("total_pass_num____ : %lliea\n", total_pass_num);
		printf("total_spilled_num_ : %iea\n", total_spilled_num);
		printf("---------------------------------------------\n");
		printf("Acc_prefilter_time : %3.1fms\n", acc_prefilter_time);
		printf("Acc_memcpy_input__ : %3.1fms\n", acc_memcpy_input_time);
		printf("Acc_cuda__________ : %3.1fms\n", acc_cuda_time);
		printf("Acc_memcpy_output_ : %3.1fms\n", acc_memcpy_output_time);
		printf("Acc_analysis_time_ : %3.1fms\n", acc_analysis_time);
		printf("Accumulated Time__ : %3.1fms\n", acc_total_time);
		printf("---------------------------------------------\n");

		fclose(pFileW);
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
