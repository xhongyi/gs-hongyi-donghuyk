/*
 * bit_mask_analyzer.h
 *
 *  Created on: Jan 27, 2012
 *      Author: hxin
 */

#include "common.h"
#include <fstream>
#include <iostream>
#include "hash_loader.h"

using namespace std;

#ifndef BIT_MASK_ANALYZER_H_
#define BIT_MASK_ANALYZER_H_

void initialization();

void unloadBitMask();

void loadBitMask(string hash_table_name, string top_bm_name,
		string bot_bm_name);

void evaluateBitMask(int upper_threshold, int lower_threshold, ofstream& outfile);

void evaluateBitMaskSingle(int hash_val, int upper_threshold, int lower_threshold, ofstream& outfile);

#endif /* BIT_MASK_ANALYZER_H_ */
