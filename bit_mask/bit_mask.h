/*
 * bitmask.h
 *
 *  Created on: Feb 8, 2012
 *      Author: hxin
 */

#ifndef BITMASK_H_
#define BITMASK_H_

enum bm_level {ZERO = 0, FIRST = 1, SECOND, THIRD, FORTH, FIFTH, SIXTH, SEVENTH};

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

struct sixth_mask {
	unsigned char mask[131072];
};

struct seventh_mask {
	unsigned char mask[1048576];
};

class bitmask {
public:
	bitmask();
	~bitmask();
	void load_mask(int hash_table_num);
	void free_mask();
	bool test_mask(int target_coor, int hash_val);
private:
	bm_info entry_bm[INDEX_NUM];
	first_mask * first_bm;
	second_mask * second_bm;
	third_mask * third_bm;
	forth_mask* forth_bm;
	fifth_mask* fifth_bm;
	sixth_mask* sixth_bm;
	seventh_mask * seventh_bm;
};





#endif /* BITMASK_H_ */
