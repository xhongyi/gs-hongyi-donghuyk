//#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "ref_db_generator.h"
#include "hash_loader.h"
#include "hash_generator.h"
//#include "hash_test.h"
#include "edit_normal.h"
//#include "edit_distribution_nofilter.h"
#include "common.h"

int main(int argc, char* argv[]) {
	char * ref_file	 = (char*) malloc(sizeof(char)*50);
	char * gen_file	 = (char*) malloc(sizeof(char)*50);
	
	strcpy(ref_file,"human_g1k_v37.fasta");
	strcpy(gen_file,"result_ref_");
//	refGenerator(gen_file, ref_file);
	
	char * file_ref	= (char*) malloc(sizeof(char)*50);
	char * file_hash	= (char*) malloc(sizeof(char)*50);
	
	for (int i = MAX_CONTIG_FILE-1; i < MAX_CONTIG_FILE ; i++) {
		sprintf(file_ref,  "%s%i", "result_ref_", i);
		sprintf(file_hash, "%s%i", "result_hash_", i);
		fprintf(stdout, "reference : %s\n", file_ref);
		fprintf(stdout, "hash table: %s\n", file_hash);
		hashTableGenerator(file_ref);
		hashFileWriter(file_hash);
	}
	free(ref_file);
	free(gen_file);
	free(file_ref);
	free(file_hash);
	return 0;
}
