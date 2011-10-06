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


/*
 * Function: Find out charaster stream with specific coordinate and size.
 * Input: result string pointer(char *result_string) / coordinate of string / size of 
 * string(size) / alignment character size(string_size)
 */
void getRefSeq(char * result_string, int coordinate, int size, int sting_size);

/*
 * Function: Generates reference sequence file with filtering illegal and 'N' character
 * It also make an alignment with specific size. It will generate reference output file.
 * Input: reference input file name(ref_file) / reference output file name(gen_file) / 
 * alignment character size(string_size)
 */
void RefGenerator(char * gen_file, char * ref_file, int string_size);

#endif /* REF_DBGENERATOR_ */

