/*
 * bitmask.h
 *
 *  Created on: Feb 8, 2012
 *      Author: hxin
 */

#ifndef BITMASK_H_
#define BITMASK_H_

enum bm_level {FIRST = 1, SECOND = 2, THIRD = 3, FORTH = 4, FIFTH = 5};

struct bm_info {
	bm_level level;
	unsigned int index;
};

struct first_mask {
	unsigned char mask;
};

struct second_mask {
	unsigned char mask[16];
};

struct third_mask {
	unsigned char mask[128];
};

struct forth_mask {
	unsigned char mask[2048];
};

struct fifth_mask {
	unsigned char mask[16384];
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
