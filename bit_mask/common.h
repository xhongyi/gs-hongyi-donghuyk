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
//#define MAX_CONTIG_FILE	25	// # of contig.
#define MAX_CONTIG_FILE	84	// # of contig.
#define MAX_FRAGMENT_LENGTH 200
//#define FRAGMENT_LENGTH		108		// # of read lenght from machine
//#define KEY_NUMBER		FRAGMENT_LENGTH/KEY_LENGTH
#define PREFILTER_SIZE		100
#define READ_SIZE		1000000
#define SKIP_BINARY_SEARCH	16777216
#define EN_SKIP_BINARY_SEARCH	true
#define MAX_ERROR_NUM		10

#include <string>

using namespace std;

extern int max_indel_num;
extern int max_diff_num;
extern bool select_cheapest;
extern int fragment_length_;
extern int key_number_;
extern bool reverse_mode;
extern bool print_detail;
extern bool bit_mask_on;

void set_fragment_length(int num);
void set_key_number(int num);

void set_reverse_mode(bool reverse_mode);

void set_select_cheapest(bool choose);

void set_bit_mask(bool choice);

void set_print_detail(bool set_print_detail);

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

struct GPU_data {
	int AF_count;
	int ED_count;
};
char reverseCompleteChar(char c);
void reverseComplete (const char *seq, char *rcSeq , int length); 

#endif /* COMMOM_H_ */
