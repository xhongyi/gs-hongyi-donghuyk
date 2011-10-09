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


char intToChar(int number);
void reconstruct_seq (char * decoded_char,int number);

int main() {
// RefGenerator Test Bench
	int string_size = 100;
	int size     	= 12;
 	char * result_string 	= (char*) malloc(size+1);
 	char * ref_file 	= (char*) malloc(sizeof(char)*4);	
 	char * gen_file 	= (char*) malloc(sizeof(char)*7);

	strcpy(ref_file,"ref");
	strcpy(gen_file,"result");
	refGenerator(gen_file, ref_file, string_size);

	#ifdef DEBUG
 	fprintf (stdout,"reference file name : %s \n", ref_file);
 	fprintf (stdout,"generated file name : %s \n", gen_file);
	#endif

// getRefSeq Test Bench
	int coordinate  = 570;

	getRefSeq(result_string, coordinate, size, string_size);	

	#ifdef DEBUG
 	fprintf (stdout,"search : %s \n", result_string);
	#endif

// Hash Reconstructor Test Bench
 	char * hash_file_name 	= (char*) malloc(sizeof(char)*20);
 	char * ref_file_name 	= (char*) malloc(sizeof(char)*20);
 	char * out_file_name 	= (char*) malloc(sizeof(char)*20);

	strcpy(hash_file_name, "testResult_");
	strcpy(ref_file_name,  "result");
	strcpy(out_file_name,  "verify_output");
	
	hashTest(hash_file_name, ref_file_name, out_file_name);

// Fee Memory Allocation 
 	free(result_string);
 	free(ref_file);
	free(gen_file);
 	return 0;
}
