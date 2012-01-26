#include "bit_mask_generator.h"
#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

int main() {
	string hash_file_name;
	string top_mask_file_name;
	string bot_mask_file_name;
	char temp[50];
	for (int i = 0; i <= 83; i++) {
		sprintf(temp, "result_hash_%d", i);
		hash_file_name = temp;
		sprintf(temp, "result_top_mask_%d", i);
		top_mask_file_name = temp;
		sprintf(temp, "result_bot_mask_%d", i);
		bot_mask_file_name = temp;
		clearBitMask();
		generateBitMask(hash_file_name);
		writeBitMask(top_mask_file_name, bot_mask_file_name);
	}
	return 0;
}
