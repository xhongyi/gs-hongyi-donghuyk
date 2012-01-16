#include "bit_mask_generator.h"
#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

int main () {
	string hash_file_name;
	string mask_file_name;
	char temp[50];
	for (int i = 0; i <= 21; i++) {
		sprintf(temp, "result_hash_%d", i);
		hash_file_name = temp;
		sprintf(temp, "result_mask_%d", i);
		mask_file_name = temp;
		clear_bit_mask();
		generate_bit_mask(hash_file_name);
		write_bit_mask(mask_file_name);
	}
}
