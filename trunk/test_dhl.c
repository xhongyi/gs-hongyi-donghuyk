//#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
//	strcpy(ref_file,"ref");
//	strcpy(gen_file,"ref_result_");
//	refGenerator(gen_file, ref_file);

// Hash File Generator
//	hashTableGenerator("ref_result_0");
//	hashFileWriter("testResult_0");

// Hash Test Testbench
//	char * hash_file_name 	= (char*) malloc(sizeof(char)*20);
//	char * ref_file_name 	= (char*) malloc(sizeof(char)*20);
//	char * out_file_name 	= (char*) malloc(sizeof(char)*20);
//	strcpy(hash_file_name, "testResult_0");
//	strcpy(ref_file_name,  "ref_result_0");
//	strcpy(out_file_name,  "verify_output_0");
//	hashTestFull(hash_file_name, ref_file_name, out_file_name);
//	hashDistribution(hash_file_name);

	char * file_ref		= (char*) malloc(sizeof(char)*20);	
	char * file_hash	= (char*) malloc(sizeof(char)*20);
	char * file_test	= (char*) malloc(sizeof(char)*20);
	for (int i = 1; i < 22 ; i++) {
		sprintf(file_ref,  "%s%i", "result_ref_", i);
		sprintf(file_hash, "%s%i", "result_hash_", i);
		sprintf(file_test, "%s%i", "result_verify_", i);
		fprintf(stdout, "reference : %s\n", file_ref);
		fprintf(stdout, "hash table: %s\n", file_hash);
		fprintf(stdout, "verify    : %s\n", file_test);
		hashTableGenerator(file_ref);
		hashFileWriter(file_hash);
	}
	for (int i = 1; i < 22 ; i++) {
		sprintf(file_ref,  "%s%i", "result_ref_", i);
		sprintf(file_hash, "%s%i", "result_hash_", i);
		sprintf(file_test, "%s%i", "result_verify_", i);
		fprintf(stdout, "reference : %s\n", file_ref);
		fprintf(stdout, "hash table: %s\n", file_hash);
		fprintf(stdout, "verify    : %s\n", file_test);
		hashDistribution(file_hash);
	}
	for (int i = 1; i < 22 ; i++) {
		sprintf(file_ref,  "%s%i", "result_ref_", i);
		sprintf(file_hash, "%s%i", "result_hash_", i);
		sprintf(file_test, "%s%i", "result_verify_", i);
		fprintf(stdout, "reference : %s\n", file_ref);
		fprintf(stdout, "hash table: %s\n", file_hash);
		fprintf(stdout, "verify    : %s\n", file_test);
		hashTestFull(file_hash, file_ref, file_test);
	}

// getRefSeq Testbench
	//int coordinate  = 570;
	//getRefSeq(result_string, coordinate, KEY_LENGTH, REF_TABLE_SIZE);	
	//#ifdef DEBUG
 	//fprintf (stdout,"search : %s \n", result_string);
	//#endif

// Hash Test Testbench
 	//char * hash_file_name 	= (char*) malloc(sizeof(char)*20);
 	//char * ref_file_name 	= (char*) malloc(sizeof(char)*20);
 	//char * out_file_name 	= (char*) malloc(sizeof(char)*20);
	//strcpy(hash_file_name, "testResult_");
	//strcpy(ref_file_name,  "result");
	//strcpy(out_file_name,  "verify_output");
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
	free(file_ref);
	free(file_hash);
	free(file_test);
	return 0;
}
