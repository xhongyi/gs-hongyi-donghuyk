#include "bit_mask_generator.h"
#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

int main () {
	string hash_file_name;
	string mask_file_name;
	char temp[50];
	for (int i = 0; i <= 83; i++) {
		sprintf(temp, "result_hash_%d", i);
		hash_file_name = temp;
		sprintf(temp, "result_mask_%d", i);
		mask_file_name = temp;
		clearBitMask();
		generateBitMask(hash_file_name);
		writeBitMask(mask_file_name);
	}
	return 0;
}
