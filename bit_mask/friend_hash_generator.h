/*
 * friend_hash_generator.h
 *
 *  Created on: Feb 24, 2012
 *      Author: hxin
 */

#ifndef FRIEND_HASH_GENERATOR_H_
#define FRIEND_HASH_GENERATOR_H_
#define MAX_SEGMENT_NUM_	16
#define MAX_INDEL_NUM_		10

#include <string>

using namespace std;

int hashVal(string key);

void initializeFriendHash();

void analyzeContig(char fragment[]);

void writeFriendHash(char store_file_name[]);

#endif /* FRIEND_HASH_GENERATOR_H_ */
