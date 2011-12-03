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
#include "book.h"

int* hash_table;
int* coordinate;
int* dev_coordinate;
int* dev_hash_table;
texture<int> tex_hash_table;
texture<int> tex_coordinate;

void loadHash(string hash_name) {
	long long coord_num = hashReconstructor(&hash_table, &coordinate, hash_name.c_str());
	HANDLE_ERROR(cudaMalloc((void**)&dev_hash_table, INDEX_NUM*sizeof(int)));
	HANDLE_ERROR(cudaMalloc((void**)&dev_coordinate, coord_num*sizeof(int)));
	HANDLE_ERROR(cudaBindTexture(NULL, tex_hash_table, dev_hash_table, INDEX_NUM*sizeof(int)));
	HANDLE_ERROR(cudaBindTexture(NULL, tex_coordinate, dev_coordinate, coord_num*sizeof(int)));
	HANDLE_ERROR(cudaMemcpy(dev_coordinate, coordinate, coord_num*sizeof(int), cudaMemcpyHostToDevice ));
	HANDLE_ERROR(cudaMemcpy(dev_hash_table, hash_table, INDEX_NUM*sizeof(int), cudaMemcpyHostToDevice ));
}

void freeHash(void) {
	HANDLE_ERROR( cudaUnbindTexture ( tex_hash_table) );
	HANDLE_ERROR( cudaUnbindTexture ( tex_coordinate) );
	HANDLE_ERROR( cudaFree ( dev_hash_table) );
	HANDLE_ERROR( cudaFree ( dev_coordinate) );
}
