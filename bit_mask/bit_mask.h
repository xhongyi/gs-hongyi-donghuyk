/*
 * bitmask.h
 *
 *  Created on: Feb 8, 2012
 *      Author: hxin
 */

#ifndef BITMASK_H_
#define BITMASK_H_

struct first_mask {
	char mask;
};

struct second_mask {
	char mask[16];
};

struct third_mask {
	char mask[128];
};

struct forth_mask {
	char mask[2048];
};

struct fifth_mask {
	char mask[16384];
};
/*
class bot_bitmask {
public:
	bot_bitmask();
	~bot_bitmask();
	bool test_mask(int target_coor, int index, );
private:
	int* bitmask;
	int array_size;
};
*/




#endif /* BITMASK_H_ */
