/*
 * hash_transfer.cpp
 *
 *  Created on: Oct 8, 2011
 *      Author: mac
 */
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <assert.h>
#include "common.h"
#include "book.h"
#include "hash_transfer.h"

int* hash_table;
int* coordinate;
string ref_string;
int* dev_coordinate;
int* dev_hash_table;
char* dev_ref_string;
texture<int> tex_hash_table;
texture<int> tex_coordinate;
texture<char> tex_ref_string;

void getHashTablePtr(int ** ptr) {
	*ptr = hash_table;
}

void getCoordinatePtr(int ** ptr) {
	*ptr = coordinate;
}

long long loadHash(string hash_name) {
	long long coord_num = hashReconstructor(&hash_table, &coordinate, hash_name.c_str());
	HANDLE_ERROR(cudaMalloc((void**)&dev_hash_table, INDEX_NUM*sizeof(int)));
	HANDLE_ERROR(cudaMalloc((void**)&dev_coordinate, coord_num*sizeof(int)));
	HANDLE_ERROR(cudaBindTexture(NULL, tex_hash_table, dev_hash_table, INDEX_NUM*sizeof(int)));
	HANDLE_ERROR(cudaBindTexture(NULL, tex_coordinate, dev_coordinate, coord_num*sizeof(int)));
	HANDLE_ERROR(cudaMemcpy(dev_coordinate, coordinate, coord_num*sizeof(int), cudaMemcpyHostToDevice ));
	HANDLE_ERROR(cudaMemcpy(dev_hash_table, hash_table, INDEX_NUM*sizeof(int), cudaMemcpyHostToDevice ));
	return coord_num;
}

int loadRef(string ref_name) {
        char tmp_ref_name[50];
        strcpy(tmp_ref_name, ref_name.c_str());
        refLoader(ref_string, tmp_ref_name);
	char *tmp_ref_string = (char*)malloc( ref_string.size() * sizeof(char));
        strcpy(tmp_ref_string, ref_string.c_str());
	HANDLE_ERROR(cudaMalloc((void**)&dev_ref_string, ref_string.size() * sizeof(char)));
        HANDLE_ERROR(cudaBindTexture(NULL, tex_ref_string, dev_ref_string, ref_string.size() * sizeof(char)));
        HANDLE_ERROR(cudaMemcpy(dev_ref_string, tmp_ref_string, ref_string.size() * sizeof(char), cudaMemcpyHostToDevice ));
        return ref_string.size();
}

void freeHash(void) {
	free (hash_table);
	free (coordinate);
	HANDLE_ERROR( cudaUnbindTexture (tex_hash_table) );
	HANDLE_ERROR( cudaUnbindTexture (tex_coordinate) );
	HANDLE_ERROR( cudaFree (dev_hash_table) );
	HANDLE_ERROR( cudaFree (dev_coordinate) );
}

void freeRef(void) {
	HANDLE_ERROR( cudaUnbindTexture (tex_ref_string) );
	HANDLE_ERROR( cudaFree (dev_ref_string) );
}

__global__ void loadHashTest(int *tmp_coordinate, int *tmp_hash_table, long long coord_num) {
	for(int i = 0; i < INDEX_NUM; i++) {
		tmp_hash_table[i] = tex1Dfetch(tex_hash_table, i);
	}
	for(int i = 0; i < coord_num; i++) {
		tmp_coordinate[i] = tex1Dfetch(tex_coordinate, i);
	}
}

__global__ void loadRefTest(char *tmp_ref_string, long long ref_num) {
	for(int i = 0; i < ref_num; i++) {
		tmp_ref_string[i] = tex1Dfetch(tex_ref_string, i);
	}
}

int ref_test(void) {
	char* tmp_ref_string;
	long long ref_num = loadRef("toy_ref_0");
	fprintf(stdout, "reference number: %lli  \n", ref_num);
	char* str_ref_string = (char*)malloc(sizeof(char)*ref_num);
	HANDLE_ERROR(cudaMalloc((void**)&tmp_ref_string, ref_num*sizeof(char)));
	loadRefTest <<<1, 1>>> (tmp_ref_string, ref_num);
	HANDLE_ERROR(cudaMemcpy(str_ref_string, tmp_ref_string, ref_num*sizeof(char), cudaMemcpyDeviceToHost ));
	bool error_ref_string = false;
	for(int i = 0; i<ref_num; i++){
		if(ref_string[i] != str_ref_string[i])
			error_ref_string = true;
	}
	if (error_ref_string == true)
		fprintf(stdout, "Ref String ERROR!! \n");
	else
		fprintf(stdout, "Ref String PASSED! \n");

	HANDLE_ERROR( cudaFree(tmp_ref_string) );
	freeRef();
	return 0;
}

int hash_test(void) {
	int* tmp_coordinate;
	int* tmp_hash_table;
	long long coord_num = loadHash("toy_hash_0");
	fprintf(stdout, "coordinate number: %lli  \n", coord_num);
	int* str_hash_table = (int*)malloc(sizeof(int)*INDEX_NUM);
	int* str_coordinate = (int*)malloc(sizeof(int)*coord_num);
	HANDLE_ERROR(cudaMalloc((void**)&tmp_hash_table, INDEX_NUM*sizeof(int)));
	HANDLE_ERROR(cudaMalloc((void**)&tmp_coordinate, coord_num*sizeof(int)));
	loadHashTest <<<1, 1>>> (tmp_coordinate, tmp_hash_table, coord_num);
	HANDLE_ERROR(cudaMemcpy(str_hash_table, tmp_hash_table, INDEX_NUM*sizeof(int), cudaMemcpyDeviceToHost ));
	HANDLE_ERROR(cudaMemcpy(str_coordinate, tmp_coordinate, coord_num*sizeof(int), cudaMemcpyDeviceToHost ));
	bool error_hash_table = false;
	bool error_coordinate = false;
	for(int i = 0; i<INDEX_NUM; i++){
		if(hash_table[i] != str_hash_table[i]) error_hash_table = true;
	}
	for(int i = 0; i<coord_num; i++){
		if(coordinate[i] != str_coordinate[i]) error_coordinate = true;
	}
	if (error_hash_table == true)   fprintf(stdout, "Hash table ERROR!! \n");
	else			    	fprintf(stdout, "Hash table PASSED! \n");
	if (error_coordinate == true)   fprintf(stdout, " Coordinate ERROR!! \n");
	else			    	fprintf(stdout, " Coordinate PASSED! \n");

	HANDLE_ERROR( cudaFree( tmp_hash_table ) );
	HANDLE_ERROR( cudaFree( tmp_coordinate ) );
	freeHash();
	return 0;
}

