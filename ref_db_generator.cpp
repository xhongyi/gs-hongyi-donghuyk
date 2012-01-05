#include "common.h"
#include <iostream>
#include "ref_db_generator.h"

void refGenerator(char * gen_file_name, char * ref_file) {
 	char * mystring  = (char*) malloc(REF_TABLE_SIZE);
 	char * newstring = (char*) malloc(REF_TABLE_SIZE+1);
	int size_ref_read = REF_TABLE_SIZE;
	int i = 0;
	int j = 0;
	int flag_ref_end = 0;
	int flag_contig_name = 0;
	int flag_first = 1;
	int file_num = 0;
	int contig_num = 0;
	int contig_file_num = 0;
	char * gen_file[200];
	char   contig_name[MAX_CONTIG_NAME];
	char   file_contig_name[MAX_CONTIG_NAME];
 	FILE * pFileR;
 	FILE * pFileW;
 	FILE * pFileW_name;
	

 	pFileR = fopen (ref_file, "r");
	sprintf(file_contig_name, "%s%s", gen_file_name, "name");
	pFileW_name = fopen (file_contig_name, "w");

	for (int flush_num = 0 ; flush_num < MAX_CONTIG_NAME ; flush_num++) {
		contig_name[flush_num] = '\0';
	}
	while (flag_ref_end == 0) {
		if (i == 0) {
			for (int flush_num = 0 ; flush_num < size_ref_read ; flush_num++) {
				mystring[flush_num] = '\0';
			}
			size_ref_read = fread (mystring, 1, REF_TABLE_SIZE, pFileR);
			i = size_ref_read;
		}
		if ((mystring[REF_TABLE_SIZE-i]=='>')||(flag_contig_name ==1)){
			if((flag_first)&&(mystring[REF_TABLE_SIZE-i]=='>')){	// First
				flag_first = 0;
				flag_contig_name = 1;
			} 
			else if (mystring[REF_TABLE_SIZE-i]=='>'){				// Others
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
			if(mystring[REF_TABLE_SIZE-i] != '>'){
				contig_name[contig_num] = mystring[REF_TABLE_SIZE-i];
				contig_num = contig_num + 1;
			}
				i = i - 1;
			if (mystring[REF_TABLE_SIZE-i] == '\n'){
//-------------------------------------------------------------------------------------
				printf("contig name:%s\n", contig_name);
				fprintf (pFileW_name, "%s\n", contig_name);
//-------------------------------------------------------------------------------------
			   	gen_file[file_num] = (char*)malloc(sizeof(gen_file_name)+file_num/10+1);
				sprintf (gen_file[file_num], "%s%i", gen_file_name, file_num);
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
			switch (mystring[REF_TABLE_SIZE-i]){
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
			if (j == REF_TABLE_SIZE) {
				newstring[j] = '\n';
				fwrite (newstring, 1, j+1, pFileW);
				j = 0;
			} 
			else if ((size_ref_read < REF_TABLE_SIZE)&&(i<=0)) {
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
 	fclose (pFileW_name);
	free (mystring);
	free (newstring);
}

string getRefSeq(int coordinate, int size,  string ref_filename) {
	string result_string(fragment_length_, 'A');
 	FILE * pFileS;
	char * search_string;
	int  size_opt;
	int  coordinate_opt;
	int  boundary;
	int  boundary_detect = 0;
	int  read_number;

	//Initialize string size
	//result_string.resize(REF_TABLE_SIZE);

	boundary   	= coordinate-(coordinate/REF_TABLE_SIZE)*REF_TABLE_SIZE + size;	// boundary checking
	size_opt   	= size + boundary / REF_TABLE_SIZE + 2;				// size optimization
	coordinate_opt 	= coordinate+coordinate/REF_TABLE_SIZE;				// add # of new line characters
 	search_string   = (char*) malloc(sizeof(char)*size_opt);	
 	pFileS = fopen (ref_filename.c_str(), "r");
	fseek (pFileS, 0, SEEK_SET );
	read_number = fread (search_string, 1, size_opt, pFileS);
	if (search_string[0] == '\n') {
		fseek (pFileS, coordinate_opt+1, SEEK_SET );
		read_number = fread (search_string, 1, size_opt, pFileS);
	} else {
		fseek (pFileS, coordinate_opt, SEEK_SET );
		read_number = fread (search_string, 1, size_opt, pFileS);
	}
	for (int i = 0; i < size ; i++) {
		if ((search_string[i+boundary_detect] == '\n')) {
			boundary_detect = boundary_detect + 1;
		} 
		result_string[i] = search_string[i+boundary_detect];
		if (boundary_detect == 2){
//cout << " DETECTED 2 BOUNDARY " << boundary_detect << endl;
		}
	}
	fclose (pFileS);
	free(search_string);
	return result_string;
}
