/*
 * edit_distance.h
 *
 *  Created on: Oct 27, 2011
 *      Author: mac
 */

#ifndef EDIT_DISTANCE_H_
#define EDIT_DISTANCE_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

enum diff_type {MISMATCH, INSERTION, DELETION};

struct ED_path {
	int path_cost[READ_LENGTH + 1];
	int front_idx;
};

struct ED_error {
	diff_type	diff;
	int			location;
	char		diff_char;
};

struct ED_result {
	bool	correct;
	int		diff_num;
	ED_error	error[MAX_ERROR_NUM];
};

/*
 * Function: Erase old data and fill the matrix with max_diff_num + 1
 */

__device__ ED_result editDistanceCal(char* test_read, char* ref_read, int key_num, ED_path* path, int main_lane, int max_indel_num, int max_diff_num);

#include "edit_distance.cu"
#endif /* EDIT_DISTANCE_H_ */
