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

void getHashTablePtr(int ** ptr) {
	*ptr = hash_table;
}

void getCoordinatePtr(int ** ptr) {
	*ptr = coordinate;
}

void loadHash(string hash_name) {
	long long coordinate_total_num = hashReconstructor(&hash_table, &coordinate, hash_name.c_str());
	HANDLE_ERROR(cudaMalloc((void**)&dev_coordinate, coordinate_total_num*sizeof(int)));
	HANDLE_ERROR(cudaMemcpy(dev_coordinate, coordinate, coordinate_total_num*sizeof(int), cudaMemcpyHostToDevice ));
}

void freeHash(void) {
	HANDLE_ERROR( cudaFree ( dev_coordinate) );
}
