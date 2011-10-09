//#define DEBUG
#include <iostream>
#include "ref_db_generator.h"
#include "hash_loader.h"
#include "hash_test.h"
#include "hash_generator.h"
#include "common.h"

void hashTest(char * hash_file_name, char * ref_file_name, char * output_file_name) {
	int  string_size = 100;
	int  size        = 12;
	int  total_index_number  = 16777216;
	int  total_file_number   = 16;
	int  fragment_pointer;
	int  fragment_number;
	int  fragment_coord;
        int  * index_db[16];
        int  * coordinate_db[16];
        char * hash_file_wIndex[16];
	char * fragment_seq     = (char*) malloc(size+1);
	char * reconstructed_seq= (char *)malloc(sizeof(char)*12);
        char * hash_file_prefix = (char *)malloc(sizeof(char)*20);
        char * decoded_char	= (char *)malloc(sizeof(char)*13);
        FILE * pFileOut;

        strcpy(hash_file_prefix, hash_file_name);
        for (int i = 0; i <10 ; i++) {
                hash_file_wIndex[i] = (char*)malloc(sizeof(hash_file_prefix)+4);
                strcpy(hash_file_wIndex[i], hash_file_name);
                hash_file_wIndex[i][sizeof(hash_file_prefix)+2] = '_';
                hash_file_wIndex[i][sizeof(hash_file_prefix)+3] = intToChar(i);
                hash_file_wIndex[i][sizeof(hash_file_prefix)+4] = '\0';
                fprintf (stdout,": %s \n", hash_file_wIndex[i]);
        }
        for (int i = 0; i <6 ; i++) {
                hash_file_wIndex[i+10] = (char*)malloc(sizeof(hash_file_prefix)+5);
                strcpy(hash_file_wIndex[i+10], hash_file_name);
                hash_file_wIndex[i+10][sizeof(hash_file_prefix)+2] = '_';
                hash_file_wIndex[i+10][sizeof(hash_file_prefix)+3] = '1';
                hash_file_wIndex[i+10][sizeof(hash_file_prefix)+4] = intToChar(i);
                hash_file_wIndex[i+10][sizeof(hash_file_prefix)+5] = '\0';
                fprintf (stdout,": %s \n", hash_file_wIndex[i+10]);
        }
        for (int i = 0; i <16 ; i++) {
                hashReconstructorChar(&index_db[i], &coordinate_db[i], hash_file_wIndex[i]);
        }

	pFileOut  = fopen (output_file_name, "w");
	for (int i = 0 ; i < total_index_number ; i++) { 
		reconstructSeq(decoded_char, i);
		fragment_pointer = index_db[hashIdx(decoded_char)][hashVal(decoded_char)]; 
		fragment_number = coordinate_db[hashIdx(decoded_char)][fragment_pointer];
		if (fragment_number != 0 ){
			fprintf (pFileOut,"\nseq %s: ", decoded_char);
			fprintf (pFileOut,"pointer %i: ", fragment_pointer);
			fprintf (pFileOut,"frag# %i----> ", fragment_number);
		}
		for (int j = 0 ; j < fragment_number ; j++) {
			fragment_coord = coordinate_db[hashIdx(decoded_char)][fragment_pointer+1+j];
			getRefSeq(fragment_seq, fragment_coord, size, string_size);
			if (strncmp(fragment_seq, decoded_char, 12) == 0) {	
				fprintf (pFileOut,"_P%i", j);
			} else {
				fprintf (pFileOut,"_F%i(%i/%s)", j, fragment_coord, fragment_seq);
			}
		}
	}
	fclose(pFileOut);

// free allocated memory
 	free(fragment_seq);
	free(reconstructed_seq);
        free(hash_file_prefix);
        free(decoded_char);
}

void reconstructSeq (char * decoded_char, int number){
	int decoded_number[13];
	int divider = 4194304;
	for (int i=0 ; i < 12 ; i++){
		decoded_number[i]=number/divider;
		number = number - decoded_number[i]*divider;
		divider = divider/4;
	}
	for (int i=0 ; i<12 ; i++){
		switch (decoded_number[i]){
			case 0: decoded_char[i] = 'A'; break;
			case 1: decoded_char[i] = 'C'; break;
			case 2: decoded_char[i] = 'G'; break;
			case 3: decoded_char[i] = 'T'; break;
			default: break;
		}
	}
	decoded_char[12] = '\0';
}

char intToChar(int number) {
	switch (number){
		case 1: return '1'; break;
		case 2: return '2'; break;
		case 3: return '3'; break;
		case 4: return '4'; break;
		case 5: return '5'; break;
		case 6: return '6'; break;
		case 7: return '7'; break;
		case 8: return '8'; break;
		case 9: return '9'; break;
		case 0: return '0'; break;
		default: break;
	}
}
