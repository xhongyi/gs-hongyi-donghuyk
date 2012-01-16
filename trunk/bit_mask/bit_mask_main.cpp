#include "bit_mask_generator.h"
#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

int main () {
	string hash_file_name;
	string mask_file_name;
	for (int i = 0; i <= 21; i++) {
		hash_file_name = sscanf("result_hash_%d", i);
		mask_file_name = sscanf("result_mask_%d", i);
		clear_bit_mask();
		generate_bit_mask(hash_file_name);
		write_bit_mask(mask_file_name);
	}
}
