/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *	  Author: mac
 */
#include <iostream>
#include "universal_header.h"

using namespace std;

__global__ void loadHashTest(int *tmp_coordinate, int *tmp_hash_table, long long coord_num, int* dev_hash_table, int* dev_coordinate) {
	for(int i = 0; i < INDEX_NUM; i++) {
		tmp_hash_table[i] = dev_hash_table[i];
	}
	for(int i = 0; i < coord_num; i++) {
		tmp_coordinate[i] = dev_coordinate[i];
	}
}

__global__ void loadRefTest(char *tmp_ref_string, long long ref_num, char *dev_ref_string) {
	for(int i = 0; i < ref_num; i++) {
		tmp_ref_string[i] = dev_ref_string[i];
	}
}

int ref_test(int ref_num) {
	char* tmp_ref_string;
//	long long ref_num = loadRef("toy_ref_0");
	fprintf(stdout, "reference number: %i  \n", ref_num);
	char* str_ref_string = (char*)malloc(sizeof(char)*ref_num);
	cudaMalloc((void**)&tmp_ref_string, ref_num*sizeof(char));
	loadRefTest <<<1, 1>>> (tmp_ref_string, ref_num, dev_ref_string);
	cudaMemcpy(str_ref_string, tmp_ref_string, ref_num*sizeof(char), cudaMemcpyDeviceToHost );
	bool error_ref_string = false;
	for(int i = 0; i<ref_num; i++){
		if(ref_string[i] != str_ref_string[i])
			error_ref_string = true;
	}
	if (error_ref_string == true)
		fprintf(stdout, "Ref String ERROR!! \n");
	else
		fprintf(stdout, "Ref String PASSED! \n");

	free(str_ref_string);
	//cudaFree(tmp_ref_string);
	//freeRef();
	return 0;
}

int hash_test(int coord_num) {
	int* tmp_coordinate;
	int* tmp_hash_table;
//	long long coord_num = loadHash("toy_hash_0");
	fprintf(stdout, "coordinate number: %i  \n", coord_num);
	int* str_hash_table = (int*)malloc(sizeof(int)*INDEX_NUM);
	int* str_coordinate = (int*)malloc(sizeof(int)*coord_num);
	cudaMalloc((void**)&tmp_hash_table, INDEX_NUM*sizeof(int));
	cudaMalloc((void**)&tmp_coordinate, coord_num*sizeof(int));
	loadHashTest <<<1, 1>>> (tmp_coordinate, tmp_hash_table, coord_num, dev_hash_table, dev_coordinate);
	cudaMemcpy(str_hash_table, tmp_hash_table, INDEX_NUM*sizeof(int), cudaMemcpyDeviceToHost );
	cudaMemcpy(str_coordinate, tmp_coordinate, coord_num*sizeof(int), cudaMemcpyDeviceToHost );
	bool error_hash_table = false;
	bool error_coordinate = false;
	for(int i = 0; i<INDEX_NUM; i++){
		if(hash_table[i] != str_hash_table[i]) {
			error_hash_table = true;
			cout << "i:" << i << " ref:" << hash_table[i] << " str:" << str_hash_table[i] << endl;
			exit(1);
		}
	}
	for(int i = 0; i<coord_num; i++){
		if(coordinate[i] != str_coordinate[i]) error_coordinate = true;
	}
	if (error_hash_table == true)
		fprintf(stdout, "Hash table ERROR!! \n");
	else
		fprintf(stdout, "Hash table PASSED! \n");
	if (error_coordinate == true)
		fprintf(stdout, "Coordinate ERROR!! \n");
	else
		fprintf(stdout, "Coordinate PASSED! \n");
	free(str_hash_table);
	free(str_coordinate);
	cudaFree( tmp_hash_table );
	cudaFree( tmp_coordinate );
	//freeHash();
	return 0;
}
