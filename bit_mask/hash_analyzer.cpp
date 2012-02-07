/*
 * hash_analyzer.cpp
 *
 *  Created on: Feb 6, 2012
 *      Author: hxin
 */
#include "common.h"
#include "hash_loader.h"
#include "hash_analyzer.h"
#include <fstream>

using namespace std;

int* hash_table;
int* coordinate;
int distribution[6];

void loadHashTable(string hash_table_name) {
	hashReconstructor(&hash_table, &coordinate, hash_table_name.c_str() );
	for (int i = 0; i < 6; i++)
		distribution[i] = 0;
}

void unloadHashTable() {
	free(hash_table);
	free(coordinate);
}

void evaluateHashTable(ofstream& outfile) {
	for (int i = 0; i < INDEX_NUM; i++) {
		int temp_threshold = 10;
		int j = 0;
		while (coordinate[hash_table[i]] > temp_threshold && j < 6) {
			distribution[j]++;
			temp_threshold *= 10;
			j++;
		}
	}
	int temp_threshold = 10;
	for (int i = 0; i < 6; i++) {
		outfile << temp_threshold << "\t" << distribution[i] << endl;
		temp_threshold *= 10;
	}
}
