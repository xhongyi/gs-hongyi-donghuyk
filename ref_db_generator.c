//#define DEBUG
#include "ref_db_generator.h"

void refGenerator(char * gen_file, char * ref_file, int string_size) {
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
			#ifdef DEBUG
			newstring[j] = '\0';
 			fprintf (stdout,"%s \n", newstring);
			#endif 
			newstring[j] = '\n';
			fwrite (newstring, 1, j+1, pFileW);
			j = 0;
		} 
		else if ((size_ref_read < string_size)&&(i<=0)) {
			#ifdef DEBUG
			//fprintf (stdout,"Here \n");
			newstring[j] = '\0';
 			fprintf (stdout,"%s \n", newstring);
			#endif
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
	int  boundary_detect = 0;
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
			boundary_detect = 1;
		}
		if (boundary_detect == 1){
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
