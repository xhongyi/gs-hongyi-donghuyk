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

int main(int argc, char* argv[]) {

	char * file_ref		= (char*) malloc(sizeof(char)*20);	
	char * file_hash	= (char*) malloc(sizeof(char)*20);
	char * file_dist	= (char*) malloc(sizeof(char)*20);
	if (argc < 2) {
		fprintf(stdout,"Need parameter [frist/cheap]\n");
		return 0;
	}
	if (!strcmp(argv[1], "first")){
		set_select_cheapest(false);
		fprintf(stdout,"Mode : First Select\n");
		sprintf(file_dist, "%s%i", "result_prefil_dist_first_", 0);
	} else if (!strcmp(argv[1], "cheap")){
		set_select_cheapest(true);
		fprintf(stdout,"Mode : Cheapest Select\n");
		sprintf(file_dist, "%s%i", "result_prefil_dist_cheap_", 0);
	} else {
		fprintf(stdout,"Error select mode [frist/cheap]\n");
		return 0;
	}
	sprintf(file_ref,  "%s%i", "result_ref_", 0);
	sprintf(file_hash, "%s%i", "result_hash_", 0);
	prefil_distribution(file_hash, file_ref, file_dist);

// Fee Memory Allocation 
	free(file_ref);
	free(file_hash);
	free(file_dist);
	return 0;
}
