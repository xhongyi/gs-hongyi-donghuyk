#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ref_db_generator.h"
#include "hash_loader.h"


int main() {
	int string_size = 100;
	int coordinate  = 570+50;
	int size     	= 200;
 	char * result_string 	= (char*) malloc(size+1);
 	char * ref_file 	= (char*) malloc(sizeof(char)*4);	
 	char * gen_file 	= (char*) malloc(sizeof(char)*7);

// RefGenerator Test Bench
	strcpy(ref_file,"ref");
	strcpy(gen_file,"result");
	refGenerator(gen_file, ref_file, string_size);
 	fprintf (stdout,"reference file name : %s \n", ref_file);
 	fprintf (stdout,"generated file name : %s \n", gen_file);

// getRefSeq Test Bench
	getRefSeq(result_string, coordinate, size, string_size);	
 	fprintf (stdout,"search : %s \n", result_string);
 	free(result_string);
 	free(ref_file);
	free(gen_file);

// Hash Reconstructor Test Bench
 	int  * index_db; 
 	int  * coordinate_db; 
 	char * hash_file 	= (char*) malloc(sizeof(char)*20);
 	char * hash_file_new 	= (char*) malloc(sizeof(char)*20);
	int  * hash_gen		= (int*)  malloc(sizeof(int)*1);
	int  * hash_read	= (int*)  malloc(sizeof(int)*10);
	int  i = 0;
	int  read_number = 0;
	FILE * pFileH;
	FILE * pFileHR;

	strcpy(hash_file,"testResult");
 	pFileH = fopen (hash_file, "w");

	(*hash_gen) = 100;
	fwrite (hash_gen, sizeof(int), 1, pFileH);
	for (i = 1; i < 100; i++) {
		(*hash_gen) = i;
		//fprintf (stdout,"hash gen : %i \n", *hash_gen);
		fwrite (hash_gen, sizeof(int), 1, pFileH);
	}
 	fclose (pFileH);

 	pFileHR = fopen (hash_file, "r");
	read_number = fread(hash_read, sizeof(int), 10, pFileHR);
	for (i = 0; i < 10; i++) {
		//fprintf (stdout,": %i ", hash_read[i]);
	}
 	fclose (pFileHR);

	//hashReconstructor(&index_db, &coordinate_db, hash_file);
	strcpy(hash_file_new,"testResult_1");
	hashReconstructor2(&index_db, &coordinate_db, hash_file_new);
	for (i = 0; i < 100 ; i++) {
		fprintf (stdout,": %i ", index_db[i]);
	}
		fprintf (stdout," \n ", index_db[i]);
	for (i = 0; i < 100 ; i++) {
		fprintf (stdout,": %i ", coordinate_db[i]);
	}
	free(hash_file);
	free(hash_gen);
	free(hash_read);
 	return 0;
}
