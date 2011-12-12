/*
 * commom.h
 *
 *  Created on: Oct 5, 2011
 *      Author: mac
 */

#ifndef COMMOM_H_
#define COMMOM_H_

#define DEBUG

#ifdef DEBUG
	# define DEBUG_PRINT1(x) 				printf(x)
	# define DEBUG_PRINT2(x, y) 			printf(x, y)
	# define DEBUG_PRINT3(x, y, z) 			printf(x, y, z)
	# define DEBUG_PRINT4(x, y, z, a) 		printf(x, y, z, a)
	# define DEBUG_PRINT5(x, y, z, a, b) 	printf(x, y, z, a, b)
	# define DEBUG_PRINT6(x, y, z, a, b, c)	printf(x, y, z, a, b, c)
#else
	# define DEBUG_PRINT1(x) 				do {} while (0)
	# define DEBUG_PRINT2(x, y)	 			do {} while (0)
	# define DEBUG_PRINT3(x, y, z) 			do {} while (0)
	# define DEBUG_PRINT4(x, y, z, a) 		do {} while (0)
	# define DEBUG_PRINT5(x, y, z, a, b) 	do {} while (0)
	# define DEBUG_PRINT6(x, y, z, a, b, c)	do {} while (0)
#endif

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
#define SKIP_BINARY_SEARCH	16777216
#define EN_SKIP_BINARY_SEARCH	true
#define MAX_ERROR_NUM		10
#define MAX_COOR_RESULT_NUM		100 //This is how many coordinate each fragment will store and return.

#define MAX_INDEL_NUM 			5
#define MAX_DIFF_NUM 			5
#define MAX_FRAGMENT_SET_NUM 	140
#define MAX_FRAGMENT_SIZE    	1680
#define MAX_THREAD_NUM			64

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
int hashVal(char* key);

/*
 * Function: 		Given a key sequence, compute the index value of which
 * 					hash table to address to.
 * Input: 			Key_sequence
 * Return: 			hash_index
 */
//int hashIdx(string key);

#endif /* COMMOM_H_ */
