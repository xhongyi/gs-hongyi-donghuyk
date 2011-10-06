#include "ref_db_generator.h"

int main() {
	int string_size = 100;
	int coordinate  = 570+50;
 	char * result_string 	= (char*) malloc(size+1);
 	char * ref_file 	= (char*) malloc(sizeof(char)*4);	
 	char * gen_file 	= (char*) malloc(sizeof(char)*7);

// RefGenerator Test Bench
	strcpy(ref_file,"ref");
	strcpy(gen_file,"result");
	RefGenerator(gen_file, ref_file, string_size);
 	fprintf (stdout,"reference file name : %s \n", ref_file);
 	fprintf (stdout,"generated file name : %s \n", gen_file);

// getRefSeq Test Bench
	int size     	= 200;
	getRefSeq(result_string, coordinate, size, string_size);	
 	fprintf (stdout,"search : %s \n", result_string);

 	free(result_string);
 	free(ref_file);
	free(gen_file);

// Hash Reconstructor Test Bench
 	int  * index_db; 
 	int  * coordinate_db; 
 	char * hash_file 	= (char*) malloc(sizeof(char)*20);
	int  * hash_gen		= (int*)  malloc(sizeof(int)*1);
	int  * hash_read	= (int*)  malloc(sizeof(int)*10);
	int  i = 0;
	int  read_number = 0;
	FILE * pFileH;
	FILE * pFileHR;

	strcpy(hash_file,"hash_table");
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

	HashReconstructor(&index_db, &coordinate_db, hash_file);
	fprintf (stdout,"aaa\n");
	for (i = 0; i < 5; i++) {
		fprintf (stdout,": %i ", index_db[i]);
	}	
	free(hash_file);
	free(hash_gen);
	free(hash_read);
 	return 0;
}

void HashReconstructor(int ** index_db, int ** coordinate_db, char * hash_table_name){
	int * total_number = (int*) malloc(sizeof(int));
	int * index = (int*) malloc(sizeof(int));
	int * data = (int*) malloc(sizeof(int));
	int read_number = 0;
	int coordinate_index = 0;
	int index_size = 5;
	int i = 0;
	int j = 0;
	FILE * pFileR;
	pFileR        = fopen (hash_table_name, "r");
	read_number   = read_number + fread(total_number, sizeof(int), 1, pFileR);
	*index_db      = malloc(sizeof(int)*index_size);
	*coordinate_db = malloc(sizeof(int)*(*total_number));
	for (i = 0; i< index_size ;i++) { 
		read_number = read_number + fread(index, sizeof(int), 1, pFileR);
		(*index_db)[i] = *index;
		(*coordinate_db)[coordinate_index] = *index;
		coordinate_index = coordinate_index + 1;
		for (j = 0; j < *index; j++){
			//fprintf (stdout,"i: %i,j: %i \n", i, j);
			read_number = read_number + fread(data,sizeof(int),1,pFileR);
			(*coordinate_db)[coordinate_index] = *data;
			coordinate_index = coordinate_index + 1;
		}
	}
	free(index);
	free(data);
	free(total_number);
	fprintf (stdout,"aaa\n");
	fclose(pFileR);
	//fprintf (stdout,": %i ", *index_db[1]);
}

void RefGenerator(char * gen_file, char * ref_file, int string_size) {
 	FILE * pFileR;
 	FILE * pFileW;
 	char * mystring  = (char*) malloc(string_size);
 	char * newstring = (char*) malloc(string_size+1);
	int size_ref_read = string_size;
	int i = 0;
	int j = 0;
	int flag_ref_end = 0;

 	pFileR = fopen (ref_file, "r");
 	pFileW = fopen (gen_file, "w");

	while (flag_ref_end == 0) {
		if (i == 0) {
			size_ref_read = fread (mystring, 1, string_size, pFileR);
			i = size_ref_read;
			//fprintf (stdout,"read size %i \n", size_ref_read);
		}
		switch (mystring[string_size-i]){
			case 'A': newstring[j]='A'; j=j+1; break;
			case 'C': newstring[j]='G'; j=j+1; break;
			case 'G': newstring[j]='C'; j=j+1; break;
			case 'T': newstring[j]='T'; j=j+1; break;
			case 'a': newstring[j]='A'; j=j+1; break;
			case 'g': newstring[j]='G'; j=j+1; break;
			case 'c': newstring[j]='C'; j=j+1; break;
			case 't': newstring[j]='T'; j=j+1; break;
			default: break;
		}
		i = i - 1;
 			//fprintf (stdout,"i: %i j:%i \n",i,j);
		if (j == string_size) {
			newstring[j] = '\0';
 			fprintf (stdout,"%s \n", newstring);
			newstring[j] = '\n';
			fwrite (newstring, 1, j+1, pFileW);
			j = 0;
		} 
		else if ((size_ref_read < string_size)&&(i<=0)) {
			//fprintf (stdout,"Here \n");
			newstring[j] = '\0';
 			fprintf (stdout,"%s \n", newstring);
			newstring[j] = '\n';
			fwrite (newstring, 1, j+1, pFileW);
			j = 0;
			flag_ref_end = 1;
		}
	}
 	fclose (pFileR);
 	fclose (pFileW);
	free (mystring);
	free (newstring);
}

void getRefSeq(char *result_string, int coordinate, int size, int string_size) {
 	FILE * pFileS;
	char * search_string;
	int  size_opt;
	int  coordinate_opt;
	int  boundary;
	int  read_number;
	int  i;

	boundary   	= coordinate-(coordinate/string_size)*string_size + size;	// boundary checking
	size_opt   	= size + boundary / string_size;				// size optimization
	coordinate_opt 	= coordinate+coordinate/string_size;				// add # of new line characters
 	search_string = (char*) malloc(size_opt+1);	
 	pFileS = fopen ("result" , "r");
	fseek (pFileS, coordinate_opt, SEEK_SET );
	read_number = fread (search_string, 1, size_opt, pFileS);
	for (i = 0; i<size ; i++) {
		if (search_string[i] == '\n') {
			result_string[i] = search_string[i+1];
		}
		else {
			result_string[i] = search_string[i];
		}
	}
	result_string[size] = '\0';	
	fclose (pFileS);
	free(search_string);
}
