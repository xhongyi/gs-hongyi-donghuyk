/*
 * edit_distance.h
 *
 *  Created on: Oct 27, 2011
 *      Author: mac
 */

#ifndef EDIT_DISTANCE_H_
#define EDIT_DISTANCE_H_

#define	KEY_LENGTH	12
#define MAX_UNFILL	255


#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

enum diff_type {MISMATCH, INSERTION, DELETION};

struct ED_constant {
	int read_length;
	int max_indel_num;
	int max_diff_num;
};

struct ED_error {
	diff_type	diff;
	int			location;
	char		diff_char;
};

struct ED_result {
	bool		correct;
	int			diff_num;
	ED_error	error[MAX_ERROR_NUM];
};

/*
 * Function: Erase old data and fill the matrix with max_diff_num + 1
 */

void fillPath(char const* test_read, char const* ref_read, int key_num, unsigned char** path, ED_constant const* cons);
#include "edit_distance.cu"
#endif /* EDIT_DISTANCE_H_ */
