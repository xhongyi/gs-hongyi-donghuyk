//#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ref_db_generator.h"
#include "hash_loader.h"
#include "hash_test.h"
#include "hash_generator.h"
#include "edit_distribution.h"
#include "edit_distribution_nofilter.h"
#include "common.h"

int main(int argc, char* argv[]) {

	char * file_ref		= (char*) malloc(sizeof(char)*50);	
	char * file_input	= (char*) malloc(sizeof(char)*50);	
	char * file_hash	= (char*) malloc(sizeof(char)*50);
	char * file_dist	= (char*) malloc(sizeof(char)*50);
	sprintf(file_ref,  "%s", "result_ref_");
	sprintf(file_hash, "%s", "result_hash_");
	sprintf(file_input, "%s", "result_input");

	if (argc < 2) {
		fprintf(stdout,"Need parameter [frist/cheap/nofilter]\n");
		return 0;
	}
	if (!strcmp(argv[1], "first")){
		set_select_cheapest(false);
		fprintf(stdout,"Mode : First Select\n");
		sprintf(file_dist, "%s", "result_dist_first_");
		edit_distribution(file_hash, file_ref, file_dist, file_input);
	} else if (!strcmp(argv[1], "cheap")){
		set_select_cheapest(true);
		fprintf(stdout,"Mode : Cheapest Select\n");
		sprintf(file_dist, "%s", "result_dist_cheap_");
		edit_distribution(file_hash, file_ref, file_dist, file_input);
	} else if (!strcmp(argv[1], "nofilter")){
		set_select_cheapest(false);
		fprintf(stdout,"Mode : Nofilter Select\n");
		sprintf(file_dist, "%s", "result_dist_nofilter_");
		edit_distribution_nofilter(file_hash, file_ref, file_dist, file_input);
	} else {
		fprintf(stdout,"Error select mode [frist/cheap/nofilter]\n");
		return 0;
	}

// Fee Memory Allocation 
	free(file_ref);
	free(file_input);
	free(file_hash);
	free(file_dist);
	return 0;
}
