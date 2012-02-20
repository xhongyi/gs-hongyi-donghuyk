/*
 * edit_distance.h
 *
 *  Created on: Oct 27, 2011
 *      Author: mac
 */

#ifndef EDIT_DISTANCE_H_
#define EDIT_DISTANCE_H_

#include "common.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

enum diff_type {MISMATCH, INSERTION, DELETION};

struct ED_error {
	diff_type	diff;
	int			location;
	char		diff_char;
};

struct ED_result {
	bool	correct;
	int		diff_num;
	ED_error	error[10];
};



/*
 * Function:	Allocate path variables, including insert_path and delete_path.
 * 				Must be called once before running editDistanceCal()
 */
void allocatePath();

void deallocatePath();

/*
 * Function: Erase old data and fill the matrix with max_diff_num + 1
 */
void initializePath();

ED_result editDistanceCal(char* test_read, char* ref_read, int key_num);

ED_result editDistanceCalFWD(char* test_read, char* ref_read, int key_num);

ED_result editDistanceCalBWD(char* test_read, char* ref_read, int key_num);

#endif /* EDIT_DISTANCE_H_ */
