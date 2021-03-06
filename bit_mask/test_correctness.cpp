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
#include "correctness_checker.h"
#include "common.h"

int main(int argc, char* argv[]) {
	char * file_ref		= (char*) malloc(sizeof(char)*20);	
	char * file_ref2	= (char*) malloc(sizeof(char)*20);	
	char * file_hash	= (char*) malloc(sizeof(char)*20);
	char * file_dist	= (char*) malloc(sizeof(char)*20);
	sprintf(file_dist, "%s", "correct_check.out");
	sprintf(file_ref,  "%s%i", "toy_ref_", 0);
	sprintf(file_ref2,  "%s%i", "toy_ref2_", 0);
	sprintf(file_hash, "%s%i", "toy_hash_", 0);
	correctness_checker(file_hash, file_ref, file_ref2, file_dist);

// Fee Memory Allocation 
	free(file_ref);
	free(file_hash);
	free(file_dist);
	return 0;
}
