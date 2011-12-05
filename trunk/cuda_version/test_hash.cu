/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: mac
 */
#include <list>
#include <iostream>
//#include "book.h"
#include "common.h"
#include "hash_transfer.h"

using namespace std;

int ref_test(void) {
	char* tmp_ref_string;
	long long ref_num = loadRef("toy_ref_0");
	fprintf(stdout, "reference number: %lli  \n", ref_num);
	char* str_ref_string = (char*)malloc(sizeof(char)*ref_num);
	cudaMalloc((void**)&tmp_ref_string, ref_num*sizeof(char));
	loadRefTest <<<1, 1>>> (tmp_ref_string, ref_num);
	cudaMemcpy(str_ref_string, tmp_ref_string, ref_num*sizeof(char), cudaMemcpyDeviceToHost );
	bool error_ref_string = false;
	for(int i = 0; i<ref_num; i++){
		if(ref_string[i] != str_ref_string[i])
			error_ref_string = true;
	}
	if (error_ref_string == true)
		fprintf(stdout, "Ref String ERROR!! \n");
	else
		fprintf(stdout, "Ref String PASSED! \n");

	cudaFree(tmp_ref_string);
	freeRef();
	return 0;
}

int main() {
	ref_test();
	hash_test();
	return 0;
}
