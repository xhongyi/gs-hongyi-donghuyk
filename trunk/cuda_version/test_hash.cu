/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: mac
 */
#include <list>
#include <iostream>
//#include "ref_db_generator.h"
#include "fragment_match.h"

using namespace std;

int main() {
	set_max_indel_num(1);
	set_max_diff_num(2);
	loadHash("toy_hash_0");
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	int  *total_num = (int*) malloc(sizeof(int));
	long *index_num = (int*) malloc(sizeof(long)*INDEX_NUM); 
	char *gen_distribution_file = (char*) malloc(sizeof(hash_table_name)+20);
	for (int l = 0 ; l < INDEX_NUM ; l++) {
		index_num[l] = 0;
	}

	int  *dev_total_num;
	long *dev_index_num;
	HANDLE_ERROR( cudaMalloc( (void**)&dev_total_num, sizeof(int) ));
	HANDLE_ERROR( cudaMalloc( (void**)&dev_index_num, sizeof(int)*INDEX_NUM ));

	HANDLE_ERROR( cudaMemcpy(total_num, dev_total_num, sizeof(int), cudaMemcpyDeviceToHost ) );
	HANDLE_ERROR( cudaMemcpy(index_num, dev_index_num, sizeof(int)*INDEX_NUM, cudaMemcpyDeviceToHost ) );
	HANDLE_ERROR( cudaFree( dev_total_num ) );
	HANDLE_ERROR( cudaFree( dev_index_num ) );
	freeHash();
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	FILE * pFileR;
	FILE * pFileOut;
	int    sum_number;
	sprintf(gen_distribution_file, "%s%s\0", hash_table_name, "_distribution");
	pFileOut = fopen (gen_distribution_file, "w");
	fprintf(pFileOut, "total_number : %i \n", total_number);
	for (int i = 0 ; i < INDEX_NUM ; i++) {
		if (index_num[i] != 0) {
			fprintf(pFileOut, "%i : %li \n", i, index_num[i]);
			sum_number = sum_number + index_num[i];
		}
	}
	fprintf(pFileOut, "sum_number : %i \n", sum_number);
	fclose(pFileOut);
	free(gen_distribution_file);
	return 0;
}
