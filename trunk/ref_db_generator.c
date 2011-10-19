#include "common.h"
#include "ref_db_generator.h"

void refGenerator(char * gen_file_name, char * ref_file, int string_size) {
 	char * mystring  = (char*) malloc(string_size);
 	char * newstring = (char*) malloc(string_size+1);
	int size_ref_read = string_size;
	int i = 0;
	int j = 0;
	int flag_ref_end = 0;
	int flag_contig_name = 0;
	int flag_first = 1;
	int file_num = 0;
	int contig_num = 0;
	int contig_file_num = 0;
	char * gen_file[100];
	char   contig_name[MAX_CONTIG_NAME];
 	FILE * pFileR;
 	FILE * pFileW;
	

 	pFileR = fopen (ref_file, "r");

	while (flag_ref_end == 0) {
		if (i == 0) {
			for (int flush_num = 0 ; flush_num < size_ref_read ; flush_num++) {
				mystring[flush_num] = '\0';
			}
			size_ref_read = fread (mystring, 1, string_size, pFileR);
			i = size_ref_read;
		}
		if ((mystring[string_size-i]=='>')||(flag_contig_name ==1)){
			if((flag_first)&&(mystring[string_size-i]=='>')){	// First
				flag_first = 0;
				flag_contig_name = 1;
			} 
			else if (mystring[string_size-i]=='>'){			// Others
                                newstring[j] = '\n';
                                fwrite (newstring, 1, j+1, pFileW);
                                j = 0;
                		fprintf (stdout,"FILE CLOSE: %s \n", gen_file[file_num]);
				fclose(pFileW);
				file_num = file_num + 1;
				flag_contig_name = 1;
				if (contig_file_num == MAX_CONTIG_FILE) {
					flag_ref_end = 1;
				}
			}
			contig_name[contig_num] = mystring[string_size-i];
			contig_num = contig_num + 1;
			i = i - 1;
			if (mystring[string_size-i] == '\n'){
               			gen_file[file_num] = (char*)malloc(sizeof(gen_file_name)+file_num/10+1);
                		sprintf (gen_file[file_num], "%s%i", gen_file_name ,file_num);
                		fprintf (stdout,"FILE OPEN : %s \n", gen_file[file_num]);
				pFileW = fopen (gen_file[file_num], "w");
				contig_file_num = contig_file_num + 1;
				flag_contig_name = 0;
				contig_num = 0;
                                newstring[j] = '\n';
                                fwrite (newstring, 1, j+1, pFileW);
                                j = 0;
			}
		} 
		else {
			if (flag_first == 1) {
               			gen_file[file_num] = (char*)malloc(sizeof(gen_file_name)+file_num/10+1);
                		sprintf (gen_file[file_num], "%s%i", gen_file_name ,file_num);
				pFileW = fopen (gen_file[file_num], "w");
				flag_first = 0;
                		fprintf (stdout,"FILE OPEN : %s \n", gen_file[file_num]);
			}
			switch (mystring[string_size-i]){
				case 'A': newstring[j]='A'; j=j+1; break;
				case 'C': newstring[j]='C'; j=j+1; break;
				case 'G': newstring[j]='G'; j=j+1; break;
				case 'T': newstring[j]='T'; j=j+1; break;
				case 'a': newstring[j]='A'; j=j+1; break;
				case 'c': newstring[j]='C'; j=j+1; break;
				case 'g': newstring[j]='G'; j=j+1; break;
				case 't': newstring[j]='T'; j=j+1; break;
				case 'N': newstring[j]='N'; j=j+1; break;
				case 'n': newstring[j]='N'; j=j+1; break;
				default: break;
			}
			i = i - 1;
			if (j == string_size) {
				newstring[j] = '\n';
				fwrite (newstring, 1, j+1, pFileW);
				j = 0;
			} 
			else if ((size_ref_read < string_size)&&(i<=0)) {
				newstring[j] = '\n';
				fwrite (newstring, 1, j+1, pFileW);
				j = 0;
				flag_ref_end = 1;
                		fprintf (stdout,"FILE CLOSE: %s \n", gen_file[file_num]);
 				fclose (pFileW);
			}
		}
	}
 	fclose (pFileR);
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

	boundary   	= coordinate-(coordinate/string_size)*string_size + size;	// boundary checking
	size_opt   	= size + boundary / string_size;				// size optimization
	coordinate_opt 	= coordinate+coordinate/string_size;				// add # of new line characters
 	search_string = (char*) malloc(size_opt+1);	
 	pFileS = fopen ("ref_result_0" , "r");
	fseek (pFileS, coordinate_opt, SEEK_SET );
	read_number = fread (search_string, 1, size_opt, pFileS);
	for (int i = 0; i<size ; i++) {
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
