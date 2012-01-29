/*
 * bm_analyzer_main.cpp
 *
 *  Created on: Jan 28, 2012
 *      Author: hxin
 */


#include "bit_mask_analyzer.h"
#include <string>

using namespace std;

int main () {
	initialization();
	for (int i = 0; i < 1/*MAX_CONTIG_FILE*/; i++) {
		char file_hash[50];
		char top_bm_file[50];
		char bot_bm_file[50];
		char file_store[50];

		sprintf(file_hash, "%s%i", "result_hash_", i);
		sprintf(top_bm_file, "%s%i", "result_top_mask_", i);
		sprintf(bot_bm_file, "%s%i", "result_bot_mask_", i);
		sprintf(file_store, "%s%i", "bm_analyze_", i);

		string hash_table_name = file_hash;
		string top_bm_name = top_bm_file;
		string bot_bm_name = bot_bm_file;

		loadBitMask(hash_table_name, top_bm_name, bot_bm_name);

		ofstream outfile;
		outfile.open(file_store);
		evaluateBitMask(1000, 100, outfile);
		outfile.close();

		unloadBitMask();
	}
	return 0;
}
