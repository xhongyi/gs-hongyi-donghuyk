/*
 * hash_loader.h
 *
 *  Created on: Oct 7, 2011
 *      Author: mac
 */

#ifndef HASH_LOADER_H_
#define HASH_LOADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * Function: Read hash table information from file and reconstruct hash table in memory.
 * Input: index_db pointer / coordinate_db pointer / hash_table_name
 * Example:
 * int * index;
 * int * coordinate;
 * HashReconstructor(&index, &coordinate, "Input");
 * ...
 */
void hashReconstructor(int ** index_db, int ** coordinate_db, char * hash_table_name);

/*
 * Function: Read hash table information from file and reconstruct hash table in memory.
 * Input: index_db pointer / coordinate_db pointer / hash_table_name
 */
void hashReconstructor2(int ** index_db, int ** coordinate_db, char * hash_table_name);


#endif /* HASH_LOADER_H_ */
