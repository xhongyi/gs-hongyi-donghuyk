/*
 * edit_distance.cpp
 *
 *  Created on: Oct 29, 2011
 *      Author: mac
 */

/*
 *
 */
#define		_MAIN_PATH_		0
#define		_DELETE_PATH_	1
#define		_INSERT_PATH_	2
#define		_UN_FILLED_		1000

#include "edit_distance.h"
#include <algorithm>
#include <cstdlib>
//#include <cstring>
#include <iostream>
#include <assert.h>
/*
 *          reference string
 * 		  a p p l i c a t i o n
 *      0 1 2
 * t  a 1 0 1 2
 * e  p 2 1 0 1 2    <----delete_path
 * s  p   2 1 0 1 2
 * t  p     2 1 1 2 3
 *    l       2 1 2 3
 * s  o     ^   2 2 3 <------main_path
 * t  c     |     3 2
 * i  a     |         2
 * n  t     |           2
 * g  i     |             2
 *    o     |               2
 *	        |
 *	    insert_path
 *
 *
 *          reference string
 *        a p p l i c a t i o n
 *      0 1 2 3
 * t  a 1 0 1 2 3
 * e  p 2 1 0 1 2 3  <----bondary_path
 * s  p 3 2 1 0 1 2 3
 * t  p   3 2 1 1 2 3 4
 *    l     3 2 1 2 3 4 5
 * s  o     ^ 3 2 2 3 4 5 <----delete_path
 * t  c     |   3 3 2 3 4
 * i  a     |       3 2 3 4
 * n  i     |         3 3 3 4
 * g  o     |             4 3 4
 *    n     |             ^ 4 3 <------main_path
 *	        |             |
 *	   bondary_path  insert_path
 *      |
 *      | | | 3 3 3 3 4 5
 *      | | 2 2 2 2 3 4 5
 *      | 1 1 1 1 2 3 4 4 4 4 4
 *      0 0 0 0 1 2 3 3 3 3 3 3
 *      1 1 1 1 1 2 2 2 3 4 4 |
 *      2 2 2 2 2 3 3 3   | | |
 *      3 3 3 3 3       | | | |
 *
 */

/*
 * Find the min edit distance givin the location.
 */
__device__ unsigned char minDistance(unsigned char const** path, int row, int column) {
	unsigned char result;
	if (path[row - 1][column] < path[row - 1][column + 1])
		result = path[row - 1][column];
	else
		result = path[row - 1][column + 1];
	
	if (path[row][column - 1] < result)
		result = path[row][column - 1];
	
	return result;
}

/*
 * Going Forward, starting from the key_num * KEY_LENGTH position and going forward until end.
 */
__device__ void fillPath(char const* test_read, char const* ref_read,
		int key_num, unsigned char** path, ED_constant const* cons) {

	//i for rows and j for columns


	//Fill up the first default row (just fill the columns)
	//For the default columns we fill 0
	for (int j = 1; j <= cons->max_indel_num; j++) {
		path[0][j] = 0;
	}
	//Starting from max_indel_num we fill numbers.
	for (int j = cons->max_indel_num + 1; j <= 2 * cons->max_indel_num + 1; j++) {
		path[0][j] = j - cons->max_indel_num - 1;
	}


	//Fill up the beginning rows
	for (int i = 1; i <= 0 + cons->max_indel_num; i++) {
		//fill 0 for imaginary columns
		for (j = 1; j <= error_num + 1 - i; j++) {
			unsigned char distance = minDistance(Path, i, j);
			path[i][j] = distance + 1;
		}

		//compare and fill the real rows
		for (j = error_num; j <= 2 * cons->max_indel_num + 1; j++) {
			unsigned char distance = minDistance(path, i, j);
			if (test_read[i - 1] == ref_read[j + i - cons->max_indel_num - 1])
				path[i][j] = distance;
			else
				path[i][j] = distance + 1;
		}
	}

	//Normal fill
	for (i = 0 + cons->max_indel_num + 1; i <= READ_LENGTH - cons->max_indel_num) {
		for (j = 1; j <= 2 * cons->max_indel_num + 1; j++) {
			unsigned char distance = minDistance(path, i, j);
			if (test_read[i - 1] == ref_read[j + i - cons->max_indel_num - 2])
				path[i][j] = distance;
			else
				path[i][j] = distance + 1;
		}
	}

	//End process. Cleaning up process
	for (i = READ_LENGTH - cons->max_indel_num + 1; i <= READ_LENGTH; i++) {
		for (j = 1; j <= READ_LENGTH + 2 + cons->max_indel_num - i; j++) {
			unsigned char distance = minDistance(path, i, j);
			if (test_read[i - 1] == ref_read[j + i - cons->max_indel_num - 2])
				path[i][j] = distance;
			else
				path[i][j] = distance + 1;
		}

		for (j = READ_LENGTH + 2 + cons->max_indel_num - i + 1;
				j <= 2 * cons->max_indel_num + 1; j++) {
			unsigned char distance = minDistance(path, i, j);
			path[i][j] = distance + 1;
		}
	}
	return;
}

//Filling backward.
/*
__device__ void fillPath(char const* test_read, char const* ref_read,
		int key_num, unsigned char** path, ED_constant const* cons) {

	int i, j;//i for rows and j for columns

	//Fill up the first default row (i = key_num * KEY_LENGTH)
	for (j = 1; j <= cons->max_indel_num; j++) {
		path[key_num * KEY_LENGTH][j] = 0;
	}

	for (j = cons->max_indel_num + 1; j <= 2 * cons->max_indel_num + 1; j++) {
		path[key_num * KEY_LENGTH][j] = j - cons->max_indel_num;
	}

	//Fill up the beginning rows
	for (i = key_num * KEY_LENGTH + 1; i <= key_num * KEY_LENGTH + cons->max_indel_num; i++) {
		//fill 0 for imaginary columns
		for (j = 1; j <= error_num - i; j++) {
			path[i][i] = 0;
		}

		//compare and fill the real rows
		for (j = error_num; j <= 2 * cons->max_indel_num + 1; j++) {
			unsigned char distance = minDistance(path, i, j);
			if (test_read[i - 1] == ref_read[j + i - cons->max_indel_num - 1])
				path[i][j] = distance;
			else
				path[i][j] = distance + 1;
		}
	}

	//Normal fill
	for (i = key_num * KEY_LENGTH + cons->max_indel_num + 1; i <= READ_LENGTH - cons->max_indel_num) {
		for (j = 1; j <= 2 * cons->max_indel_num + 1; j++) {
			unsigned char distance = minDistance(path, i, j)
			if (test_read[i - 1] == ref_read[j + i - cons->max_indel_num - 1])
				path[i][j] = distance;
			else
				path[i][j] = distance + 1;
		}
	}

	//
	return;
}

__device__ void fillPath(char const* test_read, char const* ref_read,
		int key_num, unsigned char** path, ED_constant const* cons) {

	//i for rows and j for columns

	//Fill up the first default row (i = key_num * KEY_LENGTH)
	for (int j = 2 * cons->max_indel_num + 1; j >= cons->max_indel_num; j--) {
		path[(key_num  + 1)* KEY_LENGTH][j] = 0;
	}

	for (int j = cons->max_indel_num i - 1; j >= 1; j--) {
		path[key_num * KEY_LENGTH][j] = cons->max_indel_num - j;
	}

	//Fill up the beginning rows
	for (int i = key_num * KEY_LENGTH + 1; i <= cons->max_indel_num; i--) {
		//fill 0 for imaginary columns
		for (j = 1; j <= error_num - i; j++) {
			path[i][i] = 0;
		}

		//compare and fill the real rows
		for (j = error_num; j <= 2 * cons->max_indel_num + 1; j++) {
			unsigned char distance = minDistance(path, i, j);
			if (test_read[i - 1] == ref_read[j + i - cons->max_indel_num - 1])
				path[i][j] = distance;
			else
				path[i][j] = distance + 1;
		}
	}
	return;
}

