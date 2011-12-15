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
__device__ void initializePath(ED_path* path, int main_lane, int max_indel_num);

__device__ ED_result editDistanceCalFWD(char* test_read, char* ref_read,
		int key_num, ED_path* path, int main_lane, int max_indel_num,
		int max_diff_num);

__device__ ED_result editDistanceCalBWD(char* test_read, char* ref_read,
		int key_num, ED_path* path, int main_lane, int max_indel_num,
		int max_diff_num);

__device__ void initializeFWDFront(int key_num, ED_path* path, int main_lane,
		int max_indel_num);

__device__ void initializeBWDFront(int key_num, ED_path* path, int main_lane,
		int max_indel_num);

// initializePath only fills the path elements now.
__device__ void initializePath(ED_path* path, int main_lane, int max_indel_num) {
	for (int i = 0; i < max_indel_num * 2 + 3; i++) {
		for (int j = 0; j <= READ_LENGTH; j++) {
			path[i].path_cost[j] = _UN_FILLED_;
		}
	}
}

__device__ void initializeFWDFront(int key_num, ED_path* path, int main_lane,
		int max_indel_num) {
	for (int i = 0; i < max_indel_num * 2 + 3; i++)
		//For the insertion lanes, the front point is shift right.
		path[i].front_idx = (i < main_lane) ? key_num * KEY_LENGTH + main_lane
				- i : key_num * KEY_LENGTH;
}

__device__ void initializeBWDFront(int key_num, ED_path* path, int main_lane,
		int max_indel_num) {
	for (int i = 0; i < max_indel_num * 2 + 3; i++)
		//For the insertion lanes, the front point is shift right.
		path[i].front_idx = (i > main_lane) ? key_num * KEY_LENGTH + main_lane
				- i : key_num * KEY_LENGTH;
}

__device__ ED_result editDistanceCal(char* test_read, char* ref_read,
		int key_num, ED_path* path, int main_lane, int max_indel_num,
		int max_diff_num) {

	DEBUG_PRINT1("Inside ED test 1\n");

	//Initialize path
	ED_result result;
	ED_result FWD_result, BWD_result;
	initializePath(path, main_lane, max_indel_num);
	/*----------------------------------------------------------------------
	for (int i = 0; i < max_indel_num * 2 + 3; i++) {
		for (int j = 0; j <= READ_LENGTH; j++) {
			path[i].path_cost[j] = _UN_FILLED_;
		}
	}
	----------------------------------------------------------------------*/
	DEBUG_PRINT2("FWDeditDistanceCal started  threadId: %i\n", threadIdx.x); //Idealy, We should first see all threads printing a since they are at the same warp
	FWD_result = editDistanceCalFWD(test_read, ref_read, key_num, path,
			main_lane, max_indel_num, max_diff_num);

	//__syncthreads(); //Force all threads synchronize.

	DEBUG_PRINT2("BWDeditDistanceCal started  threadId: %i\n", threadIdx.x); //We should see all threads print b. However, it is not doing that.
	BWD_result = editDistanceCalBWD(test_read, ref_read, key_num, path,
			main_lane, max_indel_num, max_diff_num);
	DEBUG_PRINT2("2 Directions have finished  threadId: %i\n", threadIdx.x);

	result.diff_num = FWD_result.diff_num + BWD_result.diff_num;

	//This is the result index used to probe the result error queue.
	int error_idx = 0;

	if (FWD_result.correct && BWD_result.correct && result.diff_num
			<= max_diff_num) {

		result.correct = true;

		for (int i = 0; i < BWD_result.diff_num; i++) {
			result.error[error_idx] = BWD_result.error[i];
			error_idx++;
		}

		for (int i = FWD_result.diff_num - 1; i >= 0; i--) {
			result.error[error_idx] = FWD_result.error[i];
			error_idx++;
		}

	} else
		result.correct = false;

	return result;
}

/*
 * Going Forward, starting from the key_num * KEY_LENGTH position and going forward until end.
 */
__device__ ED_result editDistanceCalFWD(char* test_read, char* ref_read,
		int key_num, ED_path* path, int main_lane, int max_indel_num,
		int max_diff_num) {
	//Return result;
	ED_result result;

	//Initialize the Front of each lane
	initializeFWDFront(key_num, path, main_lane, max_indel_num);

	//Current distance pointer set to 0
	int cur_dist = 0;
	//Start at the main lane
	int cur_lane = main_lane;
	//Set the first cost of the main lane to 0
	path[cur_lane].path_cost[path[cur_lane].front_idx] = 0;

	bool ED_finished = false;

	//String pointers. Used in comparing characters
	int test_idx;
	int ref_idx;

	//Do string Comparison.
	while (!ED_finished) {
		//First Pick a path.
		/*
		 * This Portion is the divergence portion, where each thread will pick a path to go through.
		 * The path pick may be different. But after a path is picked, there will be no difference.
		 * The picked path will be recorded in cur_lane.
		 */
		while (path[cur_lane].path_cost[path[cur_lane].front_idx] != cur_dist) {
			if (cur_lane == max_indel_num * 2 + 1) { //Next is boundary
				if (cur_dist >= max_diff_num) { //Check if we exceeds the max diff tolarence
					//TODO: BREAK AND END.
					ED_finished = true;
					result.correct = false;
					break;
				} else {
					//Go to next tier
					cur_lane = 1;
					cur_dist++;
				}
			} else {
				cur_lane++;
			}
		}

		//Indicate if the lane should be stopped
		int slide_stop = 0;

		//Slide down the lane.
		/*
		 * If the 2 string does not differ much, which we assume will be the common case after passing the filtering,
		 * then they would loop like 20~100 times. That's why we want to strip out the divergent portion and put it in
		 * a different path picking loop.
		 */
		while (!ED_finished) {
			//Conservative test, speed up common case
			if (path[cur_lane].front_idx >= READ_LENGTH - max_indel_num) {
				//Test if it's the last element
				if ((cur_lane <= main_lane && path[cur_lane].front_idx
						== READ_LENGTH) //Insertion lane
						|| (cur_lane > main_lane && path[cur_lane].front_idx //Deletion Lane
								== READ_LENGTH + main_lane - cur_lane)) {
					ED_finished = true;
					result.correct = true;
					break;
				}
			}

			//Compute string idx:
			test_idx = path[cur_lane].front_idx + cur_lane - main_lane;
			ref_idx = path[cur_lane].front_idx;

			//test if can slide down
			if (!(path[cur_lane].path_cost[path[cur_lane].front_idx + 1]
					== cur_dist //If can just slide
					|| test_read[test_idx] == ref_read[ref_idx]))
				slide_stop = 1;

			//Check neighbor lanes and update them. Modify -> increment -> modify
			//Check lower neighbor
			if (path[cur_lane + 1].path_cost[path[cur_lane].front_idx]
					> cur_dist + slide_stop + 1)
				path[cur_lane + 1].path_cost[path[cur_lane].front_idx]
						= cur_dist + 1;
			//Increment
			path[cur_lane].front_idx++;
			//Check itself
			if (path[cur_lane].path_cost[path[cur_lane].front_idx] > cur_dist
					+ slide_stop)
				path[cur_lane].path_cost[path[cur_lane].front_idx] = cur_dist
						+ slide_stop;
			//Check upper (right) neighbor
			if (path[cur_lane - 1].path_cost[path[cur_lane].front_idx]
					> cur_dist + slide_stop + 1)
				path[cur_lane - 1].path_cost[path[cur_lane].front_idx]
						= cur_dist + 1;

			//stop if can't slide anymore
			if (slide_stop == 1)
				break;
		}
	}
	
	DEBUG_PRINT2("inside editCalFWD, after path cost generation a  threadId: %i\n", threadIdx.x);
	DEBUG_PRINT2("inside editCalFWD, after path cost generation b  threadId: %i\n", threadIdx.x);

	//Tracing back period
	/*
	 * After you got the matrix, and all distance number, you have to go back and find out which path is the
	 * "least divergent path" and also find out where are those insertions and deletions and mismatches.
	 * The code here should not divergent much since each element has to compare it's left, up and up-left neighbor.
	 * The only divergent part is when it see an insertion or mismatch, they will have to go inside the "if block"
	 * and update next iteration position. Otherwise, it's just 3 if tests. Since the "differences" between 2 strings
	 * should be fairly small (at most 5 out of 108 comparisons). I think it's acceptiable.
	 */
	if (result.correct == false)
		return result;
	else { //If pass the test, trace back

		//char temp_result[30]; //Temp string. Used for appending.
		int cur_idx = (cur_lane <= main_lane) ? READ_LENGTH : READ_LENGTH
				+ main_lane - cur_lane;

		result.diff_num = path[cur_lane].path_cost[cur_idx];

		cur_dist = path[cur_lane].path_cost[cur_idx];

		int error_ptr = 0;

		while (cur_lane != main_lane || cur_idx != key_num * KEY_LENGTH) {

			//If we should have an insertion
			if (cur_idx == key_num * KEY_LENGTH
					|| path[cur_lane - 1].path_cost[cur_idx]
							< path[cur_lane].path_cost[cur_idx - 1]) {

				result.error[error_ptr].diff = INSERTION;
				result.error[error_ptr].location = cur_idx + cur_lane
						- main_lane - 1;
				result.error[error_ptr].diff_char
						= test_read[result.error[error_ptr].location];

				error_ptr++;
				cur_lane--;
				continue;
			}

			//If we should have a deletion
			if (path[cur_lane + 1].path_cost[cur_idx - 1]
					< path[cur_lane].path_cost[cur_idx - 1]) {

				result.error[error_ptr].diff = DELETION;
				result.error[error_ptr].location = cur_idx + cur_lane
						- main_lane - 1;
				result.error[error_ptr].diff_char = ref_read[cur_idx];

				error_ptr++;
				cur_lane++;
				cur_idx--;
				continue;
			}

			//Check if we have a mismatch
			if (path[cur_lane].path_cost[cur_idx - 1]
					< path[cur_lane].path_cost[cur_idx]) {

				result.error[error_ptr].diff = MISMATCH;
				result.error[error_ptr].location = cur_idx + cur_lane
						- main_lane - 1;
				result.error[error_ptr].diff_char
						= test_read[result.error[error_ptr].location];

				error_ptr++;
				cur_idx--;
				continue;
			}

			//Move to the next element
			cur_idx--;
		}
	}
	DEBUG_PRINT2("inside editCalFWD, after tracing back generation a  threadId: %i\n", threadIdx.x);
	DEBUG_PRINT2("inside editCalFWD, after tracing back generation b  threadId: %i\n", threadIdx.x);
	return result;
}

/*
 * BWD is backward. It's pretty much the same with forward. The difference is just sliding direction. It's going
 * backward. The stuctures are all the same. No comments here.
 */
__device__ ED_result editDistanceCalBWD(char* test_read, char* ref_read,
		int key_num, ED_path* path, int main_lane, int max_indel_num,
		int max_diff_num) {
	//Return result;
	ED_result result;

	//Initialize the Front of each lane
	initializeBWDFront(key_num, path, main_lane, max_indel_num);

	//Current distance pointer set to 0
	int cur_dist = 0;
	//Start at the main lane
	int cur_lane = main_lane;
	//Set the first cost of the main lane to 0
	path[cur_lane].path_cost[path[cur_lane].front_idx] = 0;

	bool ED_finished = false;

	//String pointers. Used in comparing characters
	int test_idx;
	int ref_idx;

	//Pick a lane to go through
	while (!ED_finished) {
		//First Pick a path.
		while (path[cur_lane].path_cost[path[cur_lane].front_idx] != cur_dist) {
			if (cur_lane == max_indel_num * 2 + 1) { //Next is boundary
				if (cur_dist >= max_diff_num) { //Check if we exceeds the max diff tolarence
					//TODO: BREAK AND END.
					ED_finished = true;
					result.correct = false;
					break;
				} else {
					//Go to next tier
					cur_lane = 1;
					cur_dist++;
				}
			} else {
				cur_lane++;
			}
		}

		//Indicate if the lane should be stopped
		int slide_stop = 0;

		//Slide down the lane.
		while (!ED_finished) {
			//Conservative test, speed up common case
			if (path[cur_lane].front_idx <= max_indel_num) {
				//Test if it's the last element
				if ((cur_lane >= main_lane && path[cur_lane].front_idx == 0) //Insertion lane
						|| (cur_lane < main_lane && path[cur_lane].front_idx //Deletion Lane
								== main_lane - cur_lane)) {
					ED_finished = true;
					result.correct = true;
					break;
				}
			}

			//Compute string idx:
			test_idx = path[cur_lane].front_idx + cur_lane - main_lane - 1;
			ref_idx = path[cur_lane].front_idx - 1;

			//test if can slide down
			if (!(path[cur_lane].path_cost[path[cur_lane].front_idx - 1]
					== cur_dist //If can just slide
					|| test_read[test_idx] == ref_read[ref_idx]))
				slide_stop = 1;

			//Check neighbor lanes and update them. Modify -> decrement -> modify
			//Check upper neighbor
			if (path[cur_lane - 1].path_cost[path[cur_lane].front_idx]
					> cur_dist + slide_stop + 1)
				path[cur_lane - 1].path_cost[path[cur_lane].front_idx]
						= cur_dist + 1;
			//Decrement
			path[cur_lane].front_idx--;
			//Check itself
			if (path[cur_lane].path_cost[path[cur_lane].front_idx] > cur_dist
					+ slide_stop)
				path[cur_lane].path_cost[path[cur_lane].front_idx] = cur_dist
						+ slide_stop;
			//Check lower (left) neighbor
			if (path[cur_lane + 1].path_cost[path[cur_lane].front_idx]
					> cur_dist + slide_stop + 1)
				path[cur_lane + 1].path_cost[path[cur_lane].front_idx]
						= cur_dist + 1;

			//stop if can't slide anymore
			if (slide_stop == 1) {
				break;
			}
		}
	}
	
	DEBUG_PRINT2("inside editCalBWD, after path cost generation a  threadId: %i\n", threadIdx.x);
	DEBUG_PRINT2("inside editCalBWD, after path cost generation b  threadId: %i\n", threadIdx.x);

	//Tracing back period
	if (result.correct == false){
DEBUG_PRINT2("1xxx: %i\n", threadIdx.x);
		return result;
	}
	else { //If pass the test, trace back
DEBUG_PRINT2("2xxx: %i\n", threadIdx.x);
		//char temp_result[30]; //Temp string. Used for appending.
		int cur_idx = (cur_lane >= main_lane) ? 0 : main_lane - cur_lane;

		result.diff_num = path[cur_lane].path_cost[cur_idx];

		cur_dist = path[cur_lane].path_cost[cur_idx];

		int error_ptr = 0;
DEBUG_PRINT2("3xxx: %i\n", threadIdx.x);		
for (int i =0; i < READ_LENGTH; i++){
	DEBUG_PRINT2("%c", test_read[i]);
}
	DEBUG_PRINT1("\n");
for (int i =0; i < READ_LENGTH; i++){
	DEBUG_PRINT2("%c", ref_read[i]);
}
	DEBUG_PRINT1("\n");

		while (cur_lane != main_lane || cur_idx != key_num * KEY_LENGTH) {
			DEBUG_PRINT2("4xxx: %i\n", threadIdx.x);
			DEBUG_PRINT2("cur_idx: %i\n", cur_idx);
			DEBUG_PRINT2("cur_idx val : %i\n", key_num * KEY_LENGTH);
			//If we should have an insertion
			if (cur_idx == key_num * KEY_LENGTH || 
				path[cur_lane + 1].path_cost[cur_idx] < path[cur_lane].path_cost[cur_idx + 1]){

				//if (error_ptr < MAX_ERROR_NUM) {	// DHL: block error
					result.error[error_ptr].diff = INSERTION;
					result.error[error_ptr].location = cur_idx + cur_lane - main_lane;
					result.error[error_ptr].diff_char = test_read[result.error[error_ptr].location];
				//}
				if (error_ptr >= MAX_ERROR_NUM) {	// DHL: block error 
					break;
				}

				error_ptr++;
				cur_lane++;
				DEBUG_PRINT4("4xx6: threadIdx %i / error_ptr: %i / cur_lane: %i \n", 
							threadIdx.x, error_ptr, cur_lane);
				continue;
			}

			//If we should have a deletion
			if (path[cur_lane - 1].path_cost[cur_idx + 1]
					< path[cur_lane].path_cost[cur_idx + 1]) {

				result.error[error_ptr].diff = DELETION;
				result.error[error_ptr].location = cur_idx + cur_lane
						- main_lane;
				result.error[error_ptr].diff_char = ref_read[cur_idx];

				error_ptr++;
				cur_lane--;
				cur_idx++;
				continue;
			}

			//Check if we have a mismatch
			if (path[cur_lane].path_cost[cur_idx + 1]
					< path[cur_lane].path_cost[cur_idx]) {
				result.error[error_ptr].diff = MISMATCH;
				result.error[error_ptr].location = cur_idx + cur_lane
						- main_lane;
				result.error[error_ptr].diff_char
						= test_read[result.error[error_ptr].location];
				error_ptr++;
				cur_idx++;
				continue;
			}

			//Move to the next element
			cur_idx++;
		}
	}
	DEBUG_PRINT2("inside editCalBWD, after tracing back generation a  threadId: %i\n", threadIdx.x);
	DEBUG_PRINT2("inside editCalBWD, after tracing back generation b  threadId: %i\n", threadIdx.x);
	return result;
}

