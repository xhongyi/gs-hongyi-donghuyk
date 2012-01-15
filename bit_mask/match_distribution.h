/*
 * hash_test.h
 *
 * Created on : Oct 7, 2011
 *
 */

#ifndef MATCH_DISTRIBUTION_
#define MATCH_DISTRIBUTION_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>

using namespace std;

/*
 * Function: Calculate Distribution of binary and prefiltering
 * Input   : hash_file_name, ref_file_name
 * output  : output_file is named by the value which sent when it is called. 
 *           Output file has the distribution of request of edit distance calcultation.
 */
void match_distribution(string hash_file_name, string ref_file_name, string output_file_name);

#endif /* MATCH_DISTRIBUTION_ */

