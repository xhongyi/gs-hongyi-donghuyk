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
#define INDEX_NUM     (1 << (2 * KEY_LENGTH ) )
#define RELEVENCE_NUM 5
#define REF_TABLE_SIZE 	100	// # of reference seqnence within one line.
#define MAX_CONTIG_NAME 400	// # of character to make contig name string.
#define MAX_CONTIG_FILE	22	// # of contig.
#define READ_LENGTH		108		// # of read lenght from machine
#define FRAGMENT_LENGTH		108		// # of read lenght from machine
#define KEY_NUMBER		READ_LENGTH/KEY_LENGTH
#define PREFILTER_SIZE		100
#define READ_SIZE		1000000

#include <string>

using namespace std;

extern int max_indel_num;
extern int max_diff_num;
extern bool select_cheapest;

void set_select_cheapest(bool choose); 

/*
 * Function: 		Set maximum indel_num that should be tolerated.
 * Modification:	max_indel_num
 */
void set_max_indel_num(int num);

/*
 * Function: 		Set maximum diff_num that should be tolerated.
 * Modification:	max_diff_num
 */
void set_max_diff_num(int num);

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
//int hashIdx(string key);

#endif /* COMMOM_H_ */
