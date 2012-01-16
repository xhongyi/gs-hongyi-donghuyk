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
void clear_bit_mask();

/*
 * Read in a hash_table file named hash_table_name
 */
void generate_bit_mask(string hash_table_name);

/*
 * Write bit mask to a file so later it can be read in as well. The file name would be bit_mask_name
 */
void write_bit_mask(string bit_mask_name);
#endif /* BIT_MAP_GENERATOR_H_ */
