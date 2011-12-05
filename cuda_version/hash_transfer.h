/*
 * hash_loader.h
 *
 *  Created on: Oct 7, 2011
 *      Author: mac
 */

#ifndef FRAGMENT_MATCH_H_
#define FRAGMENT_MATCH_H_

#include <list>
#include "hash_loader.h"
#include "common.h"
//#include "ref_db_generator.h"

#define MAX_UPPER_BOUND 10
#define MIN_LOWER_BOUND 2

using namespace std;

extern int* hash_table;
extern int* coordinate;
extern string ref_string;
extern int* dev_coordinate;
extern int* dev_hash_table;
extern char* dev_ref_string;
texture<int> tex_hash_table;
texture<int> tex_coordinate;
texture<char> tex_ref_string;

struct match_result {
	int coordinate;
	int mismatch_num;
	int insersion_num;
	int deletion_num;
	int relevance;
	bool correct;
	int key_number;
};

struct key_struct {
	int key_entry;
	int key_entry_size;
	int key_number;
	int order;
};

struct previous_coor {
	int size;
	int coor[PREFILTER_SIZE];
};

struct final_result {
	long long total_binary_search;
	long long total_edit_perform;
	long long total_correct_num;
};

void getHashTablePtr(int ** ptr);
void getCoordinatePtr(int ** ptr);
long long loadHash(string hash_name);
int loadRef(string ref_name);
void freeHash(void);
void freeRef(void);

/*
 * Function:		Search the possible locations and calculate their relevance.
 * Input:			fragment is the target gene sequence need to be mapped.
 * Modification:	result will be the top RETURN_RESULT_NUM of results that could possibly
 * 					be the mapping location.
 */
final_result searchFragment (string fragment, string* ref);

/*
 * Function:		Free the allocated memory.
 */
void freeMem();

/*
 * Function:		
 */
bool searchPrevious(int coor_value, int start_key_entry, previous_coor previous_result);

/*
 *
 */
int binary_filtering_cost(string fragment);

/*
 * Function: Get the result list without any filtering. Used in modeling original mrFAST
 */
list<match_result> noFilterSearch(string fragment);
#endif /* FRAGMENT_MATCH_H_ */
