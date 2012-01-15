/*
 * hash_test.h
 *
 * Created on : Oct 7, 2011
 *
 */

#ifndef HASH_TEST_
#define HASH_TEST_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>

using namespace std;

/*
 * Function: verifying hash table and generated reference file
 * Input: hash_table_name / generated reference file name / output file name
 * Output: generating the output file which contains verifying informain with given
 * name. 
 */
void hashTestFull(char * hash_file_name, char * ref_file_name, char * output_file_name);

/*
 * Function: change integer(1,2,3...) to character('1','2','3'...)
 * Input: single integer type number
 * Output : change the type of number to character type
 */
char intToChar(int number);

/*
 * Function: change index number (0~4^12) to sequence (AAAAAAAAAAAA ~ TTTTTTTTTTTT) 
 * Input: single integer type number, index (0~4^12)
 * output: store sequence which is matched to index number, to given name pointer (decoded_char)
 */
void reconstructSeq (char * decoded_char,int number);

#endif /* HASH_TEST_ */

