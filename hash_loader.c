//#define DEBUG
#include "common.h"
#include "hash_loader.h"

void hashReconstructor(int ** index_db, int ** coordinate_db, const char * hash_table_name){
	int *  total_number = (int*) malloc(sizeof(int));
	int *  index = (int*) malloc(sizeof(int));
	int *  data = (int*) malloc(sizeof(int));
	int    read_number = 0;
	int    coordinate_index = 0;
	FILE * pFileR;
	// Read total number of hash table
	pFileR	= fopen (hash_table_name, "r");
	read_number   = read_number + fread(total_number, sizeof(int), 1, pFileR);
	*index_db      = (int*) malloc(sizeof(int)*INDEX_NUM);
	*coordinate_db = (int*) malloc(sizeof(int)*(*total_number));
	// Read index number & coordinate based on index number
	for (int i = 0; i< INDEX_NUM ;i++) { 
		read_number = read_number + fread(index, sizeof(int), 1, pFileR);
		(*index_db)[i] = *index;
		(*coordinate_db)[coordinate_index] = *index;
		coordinate_index = coordinate_index + 1;
	// Read coordinate	
		for (int j = 0; j < *index; j++){
			read_number = read_number + fread(data,sizeof(int),1,pFileR);
			(*coordinate_db)[coordinate_index] = *data;
			coordinate_index = coordinate_index + 1;
		}
	}
	free(index);
	free(data);
	free(total_number);
	fclose(pFileR);
}

void hashReconstructorChar(int ** index_db, int ** coordinate_db, char * hash_table_name){
	int    total_number;
	int    index;
	int    data;	
	int    coordinate_index = 0;
	FILE * pFileR;

	// Read total number of hash table
	pFileR = fopen (hash_table_name, "r");
	fscanf(pFileR, "%i", &total_number);
	*index_db      = (int*) malloc(sizeof(int)*INDEX_NUM);
	*coordinate_db = (int*) malloc(sizeof(int)*total_number);

	// Read index number & coordinate based on index number
	for (int i = 0; i< INDEX_NUM ;i++) { 
		fscanf(pFileR, "%i", &index);
		(*index_db)[i] = coordinate_index;
		(*coordinate_db)[coordinate_index] = index;
		coordinate_index = coordinate_index + 1;
		
	// Read coordinate	
		for (int j = 0; j < index; j++){
			fscanf(pFileR, "%i", &data);
			(*coordinate_db)[coordinate_index] = data;
			coordinate_index = coordinate_index + 1;
		}
	}
	fclose(pFileR);
}
void hashDistribution(char * hash_table_name) {
	int    total_number;
	int    sum_number;
	int    index;
	int    data;	
	int    coordinate_index = 0;
	char * gen_distribution_file = (char*) malloc(sizeof(hash_table_name)+20);
	long int * index_num = (long int*) malloc(sizeof(long int)*INDEX_NUM);
	FILE * pFileR;
	FILE * pFileOut;

	// Read total number of hash table
	pFileR = fopen (hash_table_name, "r");
	sprintf(gen_distribution_file, "%s%s\0", hash_table_name, "_distribution");
	pFileOut = fopen (gen_distribution_file, "w");
	fscanf(pFileR, "%i", &total_number);
	fprintf(pFileOut, "total_number : %i \n", total_number);

	// Initialize index_num preser area
	for (int l = 0 ; l < INDEX_NUM ; l++) {
		index_num[l] = 0;
	}
	// Read index number & coordinate based on index number
	for (int i = 0 ; i < INDEX_NUM ; i++) { 
		fscanf(pFileR, "%i", &index);
		index_num[index] = index_num[index] + 1;
		sum_number = sum_number + 1;
	// Read coordinate	
		for (int j = 0; j < index; j++){
			fscanf(pFileR, "%i", &data);
			sum_number = sum_number + 1;
		}
	}
	for (int k = 0 ; k < INDEX_NUM ; k++) {
		if (index_num[k] != 0) {
			fprintf(pFileOut, "%i : %li \n", k, index_num[k]);
		}
	}
	fprintf(pFileOut, "sum_number : %i \n", sum_number);
	fclose(pFileOut);
	fclose(pFileR);
	free(gen_distribution_file);
	free(index_num);
}
