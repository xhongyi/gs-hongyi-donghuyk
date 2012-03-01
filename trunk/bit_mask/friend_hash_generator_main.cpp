/*
 * friend_hash_generator_main.cpp
 *
 *  Created on: Mar 1, 2012
 *      Author: hxin
 */
#include "common.h"
#include "friend_hash_generator.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

int main() {
	initializeFriendHash();
	for (int i = 0; i < MAX_CONTIG_FILE; i++) {
		char ref_name[50];
		char friend_hash_name[50];

		sprintf(ref_name, "%s%i", "result_ref_", i);
		sprintf(friend_hash_name, "%s%i", "result_friend_", i);
		analyzeContig(ref_name);
		writeFriendHash(friend_hash_name);
	}
	return 0;
}
