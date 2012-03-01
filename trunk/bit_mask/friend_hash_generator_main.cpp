/*
 * friend_hash_generator_main.cpp
 *
 *  Created on: Mar 1, 2012
 *      Author: hxin
 */
#include "friend_hash_generator.h"
#include <cstdio>
#include <cstdlib>
#include "common.h"
#include <iostream>

using namespace std;

int main() {
	initializeFriendHash();
	for (int i = 0; i < 1/*MAX_CONTIG_FILE*/; i++) {
		char ref_name[50];
		char friend_hash_name[50];

		sprintf(ref_name, "%s%i", "result_ref_", i);
		sprintf(friend_hash_name, "%s%i", "result_friend_", i);
		cout << "start analyzing" << endl;
		analyzeContig(ref_name);
		cout << "start writing files" << endl;
		writeFriendHash(friend_hash_name);
	}
	return 0;
}
