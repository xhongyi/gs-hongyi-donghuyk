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

void hashDistribution(char * hash_table_name);


/*
 * Function:	Read hash table information from file and reconstruct hash table in memory.
 * Input:		index_db pointer / coordinate_db pointer / hash_table_name
 * Example:
 * int * index;
 * int * coordinate;
 * HashReconstructorChar(&index, &coordinate, "Input"); Hash Reconstructor for char base
 * format. --> Now We use this one
 * ...
 */
void hashReconstructorChar(int ** index_db, int ** coordinate_db, char * hash_table_name);

/*
 * Function:	Read hash table information from file and reconstruct hash table in memory.
 * Input:		index_db pointer / coordinate_db pointer / hash_table_name
 */
void hashReconstructor(int ** index_db, int ** coordinate_db, char * hash_table_name);


#endif /* HASH_LOADER_H_ */
