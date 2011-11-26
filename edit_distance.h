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

struct ED_result {
	char	compare_result [30]; //Output informing how difference are the 2 strings.
	bool	correct; //Denotes if the error is within acceptable range.
};


/*
 * Function:	Allocate path variables, including insert_path and delete_path.
 * 				Must be called once before running editDistanceCal()
 */
void allocatePath();

/*
 * Function: Erase old data and fill the matrix with max_diff_num + 1
 */
void initializePath();

ED_result editDistanceCal(string test_read, string ref_read);

#endif /* EDIT_DISTANCE_H_ */
