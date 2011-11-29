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
#include <cstring>
#include <iostream>
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
 *
 *
 *
 *
 *
 *
 *
 *
 */

struct ED_path {
	int path_cost[READ_LENGTH + 1];
	int front_idx;
};

ED_path * path;

int main_lane;
int cur_lane;
int cur_dist;

void allocatePath() {
	path = new ED_path[max_indel_num * 2 + 3];
	main_lane = max_indel_num + 1;
}

void initializePath() {
	for (int i = 0; i < max_indel_num * 2 + 3; i++) {
		//For the insertion lanes, the front point is shift right.
		//
		path[i].front_idx = (i < main_lane) ? main_lane - i : 0;
		for (int j = 0; j <= READ_LENGTH; j++) {
			path[i].path_cost[j] = _UN_FILLED_;
		}
	}
}

ED_result editDistanceCal(string test_read, string ref_read) {
	//Return result;
	ED_result result;
	strcpy(result.compare_result, "\0");

	//Initialize path
	initializePath();

	//Current distance pointer set to 0
	cur_dist = 0;
	//Start at the main lane
	cur_lane = main_lane;
	//Set the first cost of the main lane to 0
	path[cur_lane].path_cost[0] = 0;

	bool ED_finished = false;

	//String pointers. Used in comparing characters
	int test_idx;
	int ref_idx;

	//Pick a lane to go through
	while (!ED_finished) {
		//cout << "Here 0" << endl;
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

		//cout << "cur_dist: " << cur_dist << endl;
		//cout << "cur_lane: " << cur_lane << endl;

		//Slide down the lane.
		while (!ED_finished) {
			//cout << "lane_front_idx: " << path[cur_lane].front_idx << endl;
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
						= cur_dist + slide_stop + 1;
			//Increment
			path[cur_lane].front_idx++;
			//Check itself
			if (path[cur_lane].path_cost[path[cur_lane].front_idx] > cur_dist
					+ slide_stop)
				path[cur_lane].path_cost[path[cur_lane].front_idx] = cur_dist
						+ slide_stop;
			//Check upper neighbor
			if (path[cur_lane - 1].path_cost[path[cur_lane].front_idx]
					> cur_dist + slide_stop + 1)
				path[cur_lane - 1].path_cost[path[cur_lane].front_idx]
						= cur_dist + slide_stop + 1;

			//stop if can't slide anymore
			if (slide_stop == 1)
				break;
		}
	}

	//cout << "Path generated" << endl;

	//Tracing back period
	if (result.correct == false)
		return result;
	else { //If pass the test, trace back
		char temp_result [30]; //Temp string. Used for appending.
		int cur_idx = (cur_lane <= main_lane) ? READ_LENGTH : READ_LENGTH
				+ main_lane - cur_lane;
		cur_dist = path[cur_lane].path_cost[cur_idx];
		int same_count = 0;
		//cout << "cur_lane: " << cur_lane << " cur_idx: " << cur_idx << endl;
		while (cur_lane != main_lane || cur_idx != 0) {
			//cout << "cur_lane: " << cur_lane << " cur_idx: " << cur_idx << endl;

			//If we should have an insertion
			if (cur_idx == 0 || path[cur_lane - 1].path_cost[cur_idx]
					< path[cur_lane].path_cost[cur_idx - 1]) {
				if (same_count != 0) {
					//cout << "Here m" << endl;
					strcpy(temp_result, result.compare_result);
					sprintf(result.compare_result, "^%c%d%s",
							test_read[cur_idx + cur_lane - main_lane - 1],
							same_count, temp_result);
				}
				else {
					//cout << "Here n" << endl;
					strcpy(temp_result, result.compare_result);
					sprintf(result.compare_result, "^%c%s",
							test_read[cur_idx + cur_lane - main_lane - 1],
							temp_result);
				}
				cur_lane--;
				same_count = 0;
				continue;
			}

			//If we should have a deletion
			if (path[cur_lane + 1].path_cost[cur_idx - 1]
					< path[cur_lane].path_cost[cur_idx - 1]) {
				if (same_count != 0) {
					//cout << "Here x" << endl;
					strcpy(temp_result, result.compare_result);
					sprintf(result.compare_result, "`%c%d%s",
							ref_read[cur_idx - 1], same_count,
							temp_result);
				} else {
					//cout << "Here y" << endl;
					strcpy(temp_result, result.compare_result);
					sprintf(result.compare_result, "`%c%s", ref_read[cur_idx - 1],
							temp_result);
				}
				cur_lane++;
				cur_idx--;
				same_count = 0;
				continue;
			}

			//Check if we have a mismatch
			if (path[cur_lane].path_cost[cur_idx - 1]
					< path[cur_lane].path_cost[cur_idx]) {
				//cout << "Here1" << endl;
				if (same_count != 0) {
					strcpy(temp_result, result.compare_result);
					sprintf(result.compare_result, "%c%d%s",
							test_read[cur_idx + cur_lane - main_lane - 1],
							same_count, temp_result);

					//cout << "Here2" << endl;
				} else {
					strcpy(temp_result, result.compare_result);
					sprintf(result.compare_result, "%c%s",
							test_read[cur_idx + cur_lane - main_lane - 1],
							temp_result);
					//cout << "Here3" << endl;
				}
				cur_idx--;
				same_count = 0;
				continue;
			}

			//Move to the next element
			cur_idx--;
			same_count++;
			//cout << "same_count: " << same_count << endl;
		}
		if (same_count != 0) { //If we have some same count at the begining
			//cout << "Here t" << endl;
			strcpy(temp_result, result.compare_result);
			sprintf(result.compare_result, "%d%s", same_count,
					temp_result);
		}
	}
	return result;
}

