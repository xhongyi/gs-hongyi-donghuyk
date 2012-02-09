/*
 * hash_analyzer_main.cpp
 *
 *  Created on: Feb 6, 2012
 *      Author: hxin
 */

#include "hash_analyzer.h"
#include "common.h"
#include <fstream>

using namespace std;

int main() {
	ofstream outfile;
	outfile.open("hash_analysis");
	outfile << "****Numbers of entries has coordinates over the threshold of: ****" << endl << endl;
	for (int i = 0; i < MAX_CONTIG_FILE; i++) {
		char file_hash[50];

		sprintf(file_hash, "%s%i", "result_hash_", i);

		string hash_table_name = file_hash;

		loadHashTable(hash_table_name);

		outfile << "**** hash table " << i << "****" << endl;

		evaluateHashTable(outfile);

		outfile << endl <<endl;

		unloadHashTable();
	}
	outfile.close();
	return 0;
}
