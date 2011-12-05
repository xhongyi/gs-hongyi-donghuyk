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
#include "cuda.h"
#include "common.h"
#include "hash_transfer.h"

int* hash_table;
int* coordinate;
string ref_string;
int* dev_coordinate;
int* dev_hash_table;
char* dev_ref_string;
//texture<int> tex_hash_table;
//texture<int> tex_coordinate;
//texture<char> tex_ref_string;

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
