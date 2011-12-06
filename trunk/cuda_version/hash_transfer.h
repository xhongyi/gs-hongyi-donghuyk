/*
 * hash_loader.h
 *
 *  Created on: Oct 7, 2011
 *      Author: mac
 */

#ifndef HASH_TRANSFER_H_
#define HASH_TRANSFER_H_

using namespace std;

extern int* hash_table;
extern int* coordinate;
extern string ref_string;
extern int* dev_coordinate;
extern int* dev_hash_table;
extern char* dev_ref_string;
texture<int> tex_hash_table;
texture<int> tex_coordinate;
texture<char> tex_ref_string;

void getHashTablePtr(int ** ptr);
void getCoordinatePtr(int ** ptr);
long long loadHash(string hash_name);
int loadRef(string ref_name);
void freeHash(void);
void freeRef(void);

__global__ void loadHashTest(int *tmp_coordinate, int *tmp_hash_table, long long coord_num, int* dev_hash_table, int* dev_coordinate);

__global__ void loadRefTest(char *tmp_ref_string, long long ref_num, char *dev_ref_string);

int hash_test(void);

int ref_test(void);

#include "hash_transfer.cu"
#endif /* HASH_TRANSFER_H_ */
