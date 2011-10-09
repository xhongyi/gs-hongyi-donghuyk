//#define DEBUG
#include "hash_loader.h"

void hashReconstructor_char(int ** index_db, int ** coordinate_db, char * hash_table_name){
	int * total_number = (int*) malloc(sizeof(int));
	int * index = (int*) malloc(sizeof(int));
	int * data = (int*) malloc(sizeof(int));
	char * char_each   = (char*) malloc(sizeof(char));
	char * char_stream = (char*) malloc(sizeof(char)*10);
	int read_number = 1;
	int total_char_number = 0;
	int coordinate_index = 0;
	int index_size = 4*4*4*4*4*4*4*4*4*4;
	int index_sum = 0;
	int i = 0;
	int ii= 0;
	int kk= 0;
	int j = 0;
	int number  	 = 0;
	int first_flag   = 1;
	int end_detector = 0;
	int index_update = 0;
	int total_data_size = 100;
	FILE * pFileR;
	pFileR = fopen (hash_table_name, "r");
	i = 0;
	#ifdef DEBUG
	fprintf (stdout,"file: %s\n", hash_table_name);
	#endif
	total_char_number = 0;

	for (ii = 0; ii< total_data_size+1 ;ii++) { 
		i = 0;
		while (!end_detector) {
			read_number   = fread(char_each, sizeof(char), 1, pFileR);
			if ((*char_each=='\n')||(*char_each=='\0')||(*char_each==' ')){
				end_detector = 1;
				total_char_number = i;
			}
			char_stream[i] = *char_each;
			i = i + 1;
			if (((*char_each=='\n')||(*char_each=='\0')||(*char_each==' '))&&(i==1)){
				end_detector = 0;
				i = 0;
			}
		}
		end_detector = 0;
		*total_number = 0;
		for(i = 0; i < total_char_number; i++) {
			switch (char_stream[i]){
				case '0': number=0; break;
				case '1': number=1; break;
				case '2': number=2; break;
				case '3': number=3; break;
				case '4': number=4; break;
				case '5': number=5; break;
				case '6': number=6; break;
				case '7': number=7; break;
				case '8': number=8; break;
				case '9': number=9; break;
				default: 	    break;
			}
			for (j = 1 ; j < total_char_number-i ; j++){
				number = number*10;
			}
			*total_number = *total_number + number;
		}
		if (first_flag == 1) {
			*index_db      = (int*) malloc(sizeof(int)*index_size);
			*coordinate_db = (int*) malloc(sizeof(int)*(*total_number));
			first_flag = 0;
			total_data_size = *total_number;
			#ifdef DEBUG
			fprintf (stdout,"mamory allocate index:%i coordinate:%i\n", index_size, *total_number);
			#endif
		} else {
			if (index_update==0){
				(*index_db)[kk] = coordinate_index; //(*index_db)[kk] = *total_number;
				index_update    = *total_number;
				#ifdef DEBUG
				fprintf (stdout,"index: %i # of index:%i \n", (*index_db)[kk], kk);
				#endif
				kk = kk +1;
			} else { 
				index_update = index_update - 1;
			}
			(*coordinate_db)[coordinate_index] = *total_number;
			#ifdef DEBUG
			fprintf (stdout,"coordinate: %i # of coordinate:%i \n", (*coordinate_db)[coordinate_index], coordinate_index);
			#endif
			coordinate_index = coordinate_index + 1;
		}
	}
	free(index);
	free(data);
	free(total_number);
	fclose(pFileR);
}

void hashReconstructor(int ** index_db, int ** coordinate_db, char * hash_table_name){
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
	*index_db      = (int*) malloc(sizeof(int)*index_size);
	*coordinate_db = (int*) malloc(sizeof(int)*(*total_number));
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
	fclose(pFileR);
	//fprintf (stdout,": %i ", *index_db[1]);
}

