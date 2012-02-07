/*
 * hash_analyzer.h
 *
 *  Created on: Feb 6, 2012
 *      Author: hxin
 */

#ifndef HASH_ANALYZER_H_
#define HASH_ANALYZER_H_

#include <string>

using namespace std;

void loadHashTable(string hash_table_name);

void unloadHashTable();

void evaluateHashTable(ofstream& outfile);

#endif /* HASH_ANALYZER_H_ */
