//#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "ref_db_generator.h"
#include "hash_loader.h"
#include "hash_test.h"
#include "hash_generator.h"
#include "common.h"

int main() {
// RefGenerator Testbench
 	char * result_string 	= (char*) malloc(KEY_LENGTH+1);
 	char * ref_file 	= (char*) malloc(sizeof(char)*4);	
 	char * gen_file 	= (char*) malloc(sizeof(char)*7);

//	strcpy(ref_file,"human_g1k_v37.fasta");
	strcpy(ref_file,"ref");
	strcpy(gen_file,"db_result_");
	refGenerator(gen_file, ref_file, REF_TABLE_SIZE);

	#ifdef DEBUG
 	fprintf (stdout,"reference file name : %s \n", ref_file);
 	fprintf (stdout,"generated file name : %s \n", gen_file);
	#endif

// getRefSeq Testbench
	int coordinate  = 570;

	//getRefSeq(result_string, coordinate, KEY_LENGTH, REF_TABLE_SIZE);	

	#ifdef DEBUG
 	//fprintf (stdout,"search : %s \n", result_string);
	#endif

// Hash Test Testbench
 	char * hash_file_name 	= (char*) malloc(sizeof(char)*20);
 	char * ref_file_name 	= (char*) malloc(sizeof(char)*20);
 	char * out_file_name 	= (char*) malloc(sizeof(char)*20);

	strcpy(hash_file_name, "testResult_");
	strcpy(ref_file_name,  "result");
	strcpy(out_file_name,  "verify_output");
	//hashTest(hash_file_name, ref_file_name, out_file_name);

// Hash Reconstructor Testbench
	//int  * index_db;
	//int  * coordinate_db;
	//char hash_file_wIndex[] = "testResult_0";

	//hashReconstructorChar(&index_db, &coordinate_db, hash_file_wIndex);
	//fprintf(stdout,"index %i / coord %i \n", index_db[0], coordinate_db[0]);


// Fee Memory Allocation 
 	free(result_string);
 	free(ref_file);
	free(gen_file);
 	return 0;
}
