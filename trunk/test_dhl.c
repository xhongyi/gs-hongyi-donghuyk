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
//	strcpy(gen_file,"result_18_ref_");
//	refGenerator(gen_file, ref_file);

	char * file_ref		= (char*) malloc(sizeof(char)*20);	
	char * file_hash	= (char*) malloc(sizeof(char)*20);
	char * file_test	= (char*) malloc(sizeof(char)*20);

	
	sprintf(file_ref,  "%s%i", "result_9_ref_", 0);
	sprintf(file_hash, "%s%i", "result_9_hash_", 0);
	sprintf(file_test, "%s%i", "result_9_verify_", 0);
	hashTableGenerator(file_ref);
	hashFileWriter(file_hash);
	hashDistribution(file_hash);
	hashTestFull(file_hash, file_ref, file_test);

//	for (int i = 9; i < 22 ; i++) {
//		sprintf(file_ref,  "%s%i", "result_ref_", i);
//		sprintf(file_hash, "%s%i", "result_hash_", i);
//		sprintf(file_test, "%s%i", "result_verify_", i);
//		fprintf(stdout, "reference : %s\n", file_ref);
//		fprintf(stdout, "hash table: %s\n", file_hash);
//		fprintf(stdout, "verify    : %s\n", file_test);
//
//		hashTableGenerator(file_ref);
//		hashFileWriter(file_hash);
//		hashDistribution(file_hash);
//	}
//	for (int i = 9; i < 22 ; i++) {
//		sprintf(file_ref,  "%s%i", "result_ref_", i);
//		sprintf(file_hash, "%s%i", "result_hash_", i);
//		sprintf(file_test, "%s%i", "result_verify_", i);
//		fprintf(stdout, "reference : %s\n", file_ref);
//		fprintf(stdout, "hash table: %s\n", file_hash);
//		fprintf(stdout, "verify    : %s\n", file_test);
//
//		hashTestFull(file_hash, file_ref, file_test);
//	}

// Fee Memory Allocation 
 	free(result_string);
 	free(ref_file);
	free(gen_file);
	free(file_ref);
	free(file_hash);
	free(file_test);
	return 0;
}
