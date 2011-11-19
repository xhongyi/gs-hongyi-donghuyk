/*
 * fragment_match.h
 *
 *  Created on: Oct 7, 2011
 *      Author: mac
 */

#ifndef FRAGMENT_MATCH_H_
#define FRAGMENT_MATCH_H_

#include <list>
#include "hash_loader.h"
#include "common.h"
#include "ref_db_generator.h"

#define MAX_UPPER_BOUND 10
#define MIN_LOWER_BOUND 2

using namespace std;

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

/*
 * Function:		Loading the hash_table into the main memory from the file. Giving
 *					the name of a chromosome, it will load all the hash_table this
 *					chromosome has. The chromosome file should be already organized
 *					as "hash_name"_"num", ex. chr1_0, with chr1 is the name of the
					chromosome, 0 is the first file.
 * Input:			hash_name is the name of the hash table of the chromosome.
 * Modification:	It will load all the hash_table into hash_table and coordinate.
 */
void loadHash(string hash_name);

/*
 * Function:		Search the possible locations and calculate their relevance.
 * Input:			fragment is the target gene sequence need to be mapped.
 * Modification:	result will be the top RETURN_RESULT_NUM of results that could possibly
 * 					be the mapping location.
 */
list<match_result> searchFragment (string fragment);

/*
 * Function:		Binary search if there exits an exact match on a target coordinate.
 * Input:			key is the searching target, and coordinate will be the target coordinate.
 * Modification:	result_idx will be changed to the index of the first possible match.
 * 					result_scope will be changed to the number of possible matches.
 */


/*
 * Function:		Free the allocated memory.
 */
void freeMem();

/*
 * Function:		
 */
bool searchPrevious(int coor_value, int start_key_entry, list<match_result> previous_result);

/*
 *
 */
int binary_filtering_cost(string fragment);
#endif /* FRAGMENT_MATCH_H_ */
