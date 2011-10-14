/*
 * commom.h
 *
 *  Created on: Oct 5, 2011
 *      Author: mac
 */

#ifndef COMMOM_H_
#define COMMOM_H_

#define KEY_LENGTH 12
//How many files hash table should break up to. Must be pow of 4
#define INDEX_NUM     (1 << (2 * (KEY_LENGTH - 2) ) )
#define RELEVENCE_NUM 5
#define REF_TABLE_SIZE 100
#define FRAGMENT_MAX_LENGTH 200
#define MAX_CONTIG_NAME     400
#define MAX_CONTIG_NUMBER   100
#define MAX_CONTIG_FILE	     22

#include <string>

using namespace std;

//int max_indel_num = 0;

/*
 * Function: 		Set maximum indel_num that should be tolerated.
 * Modification:	indel_num
 */
//void set_max_indel_num(int num);

/*
 * Function:		Given a key sequence, compute the hash table value
 * Input: 			Key_sequence
 * Return: 			hash_value
 */
int hashVal(string key);

/*
 * Function: 		Given a key sequence, compute the index value of which
 * 					hash table to address to.
 * Input: 			Key_sequence
 * Return: 			hash_index
 */
int hashIdx(string key);

#endif /* COMMOM_H_ */
