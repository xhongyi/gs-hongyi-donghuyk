/*
 * bit_map_generator.h
 *
 *  Created on: Jan 15, 2012
 *      Author: mac
 */

#ifndef BIT_MAP_GENERATOR_H_
#define BIT_MAP_GENERATOR_H_

#include "common.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

extern int bit_mask[INDEX_NUM];

/*
 * Fill zeros in to the array
 */
void clearBitMask();

/*
 * Read in a hash_table file named hash_table_name
 */
void generateBitMask(string hash_table_name);

/*
 * Write bit mask to a file so later it can be read in as well. The file name would be bit_mask_name
 */
void writeBitMask(string top_bit_mask_name, string bot_bit_mask_name);
#endif /* BIT_MAP_GENERATOR_H_ */
