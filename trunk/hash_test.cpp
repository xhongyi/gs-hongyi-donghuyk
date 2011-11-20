//#define DEBUG
#include <iostream>
#include "ref_db_generator.h"
#include "hash_loader.h"
#include "hash_test.h"
#include "hash_generator.h"
#include "common.h"

void hashTestFull(char * hash_file_name, char * ref_file_name,
		char * output_file_name) {
	int error_count = 0;
	int total_error_count = 0;
	int fragment_pointer;
	int fragment_number;
	int fragment_coord;
	int * index_db;
	int * coordinate_db;
	string fragment_seq;
	char * reconstructed_seq = (char *) malloc(sizeof(char) * KEY_LENGTH);
	char * decoded_char = (char *) malloc(sizeof(char) * (KEY_LENGTH + 1));
	FILE * pFileOut;
	int* prefilter;
	fprintf(stdout, "Read Hash Table: %s \n", hash_file_name);
	hashReconstructor(&index_db, &coordinate_db, &prefilter, hash_file_name);
	pFileOut = fopen(output_file_name, "w");
	for (int i = 0; i < INDEX_NUM; i++) {
		reconstructSeq(decoded_char, i);
		fragment_pointer = index_db[hashVal(decoded_char)];
		fragment_number  = coordinate_db[fragment_pointer];
		if (fragment_number != 0) {
			fprintf(pFileOut, "\nseq %s: pointer %i: frag# %i---->",
					decoded_char, fragment_pointer, fragment_number);
		}
		for (int j = 0; j < fragment_number; j++) {
			fragment_coord = coordinate_db[fragment_pointer + 1 + j];
			fragment_seq = getRefSeq(fragment_coord, KEY_LENGTH, ref_file_name);
			if (strncmp(fragment_seq.c_str(), decoded_char, KEY_LENGTH) == 0) {
			//	fprintf(pFileOut, "_P%i", j );
			} else {
				fprintf(pFileOut, "F%i(%i/%s)_", j, fragment_coord, fragment_seq.c_str() );
				error_count = error_count + 1;
			}
		}
		if ((error_count == 0)&&(fragment_number != 0)) {
			fprintf(pFileOut, "ALL PASS");
		}
		total_error_count = total_error_count + error_count;
		error_count = 0;
	}
	if (total_error_count == 0) {
		fprintf(pFileOut, "\nFinally, ALL PASS\n");
	} 

	fclose(pFileOut);
	// free allocated memory
	free(reconstructed_seq);
	free(decoded_char);
}

void reconstructSeq(char * decoded_char, int number) {
	int decoded_number[KEY_LENGTH + 1];
	int divider = INDEX_NUM / 4;
	for (int i = 0; i < KEY_LENGTH; i++) {
		decoded_number[i] = number / divider;
		number = number - decoded_number[i] * divider;
		divider = divider / 4;
	}
	for (int i = 0; i < KEY_LENGTH; i++) {
		switch (decoded_number[i]) {
		case 0:
			decoded_char[i] = 'A';
			break;
		case 1:
			decoded_char[i] = 'C';
			break;
		case 2:
			decoded_char[i] = 'G';
			break;
		case 3:
			decoded_char[i] = 'T';
			break;
		default:
			break;
		}
	}
	decoded_char[KEY_LENGTH] = '\0';
}

char intToChar(int number) {
	switch (number) {
	case 1:
		return '1';
		break;
	case 2:
		return '2';
		break;
	case 3:
		return '3';
		break;
	case 4:
		return '4';
		break;
	case 5:
		return '5';
		break;
	case 6:
		return '6';
		break;
	case 7:
		return '7';
		break;
	case 8:
		return '8';
		break;
	case 9:
		return '9';
		break;
	case 0:
		return '0';
		break;
	default:
		break;
	}
}
