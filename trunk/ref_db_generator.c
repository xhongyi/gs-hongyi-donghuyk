#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
void getRefSeq(char * result_string, int coordinate, int size, int sting_size);
void RefGenerator(char * gen_file, char * ref_file, int string_size);

int main() {
	int string_size = 30;
	int coordinate  = 570+50;
	int size     	= 200;
 	char * result_string 	= (char*) malloc(size+1);
 	char * ref_file 	= (char*) malloc(sizeof(char)*4);	
 	char * gen_file 	= (char*) malloc(sizeof(char)*7);

	gen_file[0] = 'r';
	gen_file[1] = 'e';
	gen_file[2] = 'f';
	gen_file[0] = 'r';
	gen_file[1] = 'e';
	gen_file[2] = 's';
	gen_file[3] = 'u';
	gen_file[4] = 'l';
	gen_file[5] = 't';
	RefGenerator(gen_file, ref_file, string_size);
	getRefSeq(result_string, coordinate, size, string_size);	
 	fprintf (stdout,"search : %s \n", result_string);

 	fprintf (stdout,"reference file name : %s \n", ref_file);
 	fprintf (stdout,"generated file name : %s \n", gen_file);
 	free(result_string);
 	free(ref_file);
	free(gen_file);
 	return 0;
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