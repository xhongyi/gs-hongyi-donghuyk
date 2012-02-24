/*
 * friend_hash_generator.cpp
 *
 *  Created on: Feb 23, 2012
 *      Author: hxin
 */
#include "friend_hash_generator.h"
#include "string"

bool friend_hash[INDEX_NUM][MAX_SEGMENT_NUM][MAX_INDEL_NUM + 1];

void analyze_fragment(string fragment) {
	string first_half = fragment.substr(0, 12);

	//If the first have has bad key
	if (first_half.find('N') != string::npos)
		return;

	for (int i = 0; i < MAX_SEGMENT_NUM; i++) {
		for (int j = 0; j <= MAX_INDEL_NUM; j++)
	}
}



