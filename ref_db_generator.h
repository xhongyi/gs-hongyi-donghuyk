/*
 * ref_db_generator.h
 *
 * Created on : Oct 5, 2011
 *
 */

#ifndef REF_DB_GENERATOR_
#define REF_DB_GENERATOR_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>

using namespace std;

/*
 * Function: Find out charaster stream with specific coordinate and size.
 * Input: coordinate of string / size of
 * string(size) 
 */
string getRefSeq(int coordinate, int size, string ref_filename);

/*
 * Function: Generates reference sequence file with filtering illegal and 'N' character
 * It also make an alignment with specific size. It will generate reference output file.
 * Input: reference input file name(ref_file) / reference output file name(gen_file) / 
 */
void refGenerator(char * gen_file, char * ref_file);

#endif /* REF_DBGENERATOR_ */

