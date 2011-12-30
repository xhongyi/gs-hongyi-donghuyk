//#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "common.h"
#include "ref_db_generator.h"
#include "hash_loader.h"
#include "hash_generator.h"
#include "edit_normal.h"

int main(int argc, char* argv[]) {
	if (argc >= 2) {
		char temp = *argv[1];
		set_max_indel_num(atoi(&temp));
		set_max_diff_num(atoi(&temp));
		fprintf(stdout,"Max indel & diff : %d\n", atoi(&temp));
	} 
	else {
		set_max_indel_num(3);
		set_max_diff_num(3);
		fprintf(stdout,"Max indel & diff : 3\n");
	}
	char * file_ref	= (char*) malloc(sizeof(char)*50);	
	char * file_input	= (char*) malloc(sizeof(char)*50);	
	char * file_hash	= (char*) malloc(sizeof(char)*50);
	char * file_dist	= (char*) malloc(sizeof(char)*50);
	sprintf(file_ref,  "%s", "result_ref_");
	sprintf(file_hash, "%s", "result_hash_");
	sprintf(file_input, "%s", "result_input");
	set_select_cheapest(true);
	fprintf(stdout,"Mode : Alignment\n");
	sprintf(file_dist, "%s", "result_dist_cheap_");
	edit_normal(file_hash, file_ref, file_dist, file_input);
	free(file_ref);
	free(file_input);
	free(file_hash);
	free(file_dist);
	return 0;
}
