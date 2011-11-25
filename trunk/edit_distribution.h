/*
 * hash_test.h
 *
 * Created on : Oct 7, 2011
 *
 */

#ifndef EDIT_DISTRIBUTION_
#define EDIT_DISTRIBUTION_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>

using namespace std;

/*
 * Function: Distribution of matched one after whole things, binary filtering / prefiltering / edit-distance calculation. 
 * Input   : hash_file_name / reference file name / reference file name 2 <- This is duplicated file of original referen
	     files
 * output  : The distribution of matched one which is matched within tolerant factor.
 */
void edit_distribution(string hash_file_name, string ref_file_name, string output_file_name, string result_input); 

#endif /* EDIT_DISTRIBUTION_ */
