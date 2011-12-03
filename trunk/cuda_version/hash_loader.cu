//#define DEBUG
#include "common.h"
#include "hash_loader.h"
#include <fstream>
long long hashReconstructor(int ** index_db, int ** coordinate_db, const char * hash_table_name) { 
	int total_number;
	int index;
	int data;
	int coordinate_index = 0;
	int max_coordinate_num = 2;
	FILE * pFileR;

	// Read total number of hash table
	pFileR = fopen(hash_table_name, "r");
	if (pFileR == NULL) {
		printf("Error Open File : %s\n", hash_table_name);
	}
	fscanf(pFileR, "%i", &total_number);
	*index_db = (int*) malloc(sizeof(int) * INDEX_NUM);
	*coordinate_db = (int*) malloc(sizeof(int) * total_number);

	// Read index number & coordinate based on index number
	for (int i = 0; i < INDEX_NUM; i++) {
		fscanf(pFileR, "%i", &index);
		(*index_db)[i] = coordinate_index;
		(*coordinate_db)[coordinate_index] = index;
		coordinate_index = coordinate_index + 1;
		// Read coordinate
		for (int j = 0; j < index; j++) {
			fscanf(pFileR, "%i", &data);
			(*coordinate_db)[coordinate_index] = data;
			coordinate_index = coordinate_index + 1;
		}
	}
	fclose(pFileR);
	return coordinate_index + 1;
}

void refLoader(string & ref_db, char * ref_name) {
	ref_db.clear();
	ifstream ref_file;
	ref_file.open(ref_name, fstream::in);
	string temp_read;
	while (true) {
		ref_file >> temp_read;
		if (!ref_file.eof() )
		ref_db += temp_read;
		else
			break;
	}
}
