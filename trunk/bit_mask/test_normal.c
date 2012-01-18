//#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "common.h"
#include "ref_db_generator.h"
#include "hash_loader.h"
#include "hash_generator.h"
#include "edit_normal.h"

int main(int argc, char* argv[]) {
	if (argc >= 4) {
		char temp = *argv[1];
		set_max_indel_num(atoi(&temp));
		set_max_diff_num(atoi(&temp));
		if (!strcmp(argv[2], "simple")){
			set_print_detail(false);
			fprintf(stdout,"Output Mode : simple\n");
		}
		else if (!strcmp(argv[2], "detail")){
			set_print_detail(true);
			fprintf(stdout,"Output Mode : detail\n");
		} 
		else {
			set_print_detail(true);
			fprintf(stdout,"Output Mode : detail\n");
		}
		if (!strcmp(argv[3], "bm_on") ) {
			set_bit_mask(true);
			fprintf(stdout, "Bit mask: on\n");
		}
		else {
			set_bit_mask(false);
			fprintf(stdout, "Bit mask: off\n");
		}
		char print_detail = *argv[2];
		fprintf(stdout,"Max indel & diff : %d\n", atoi(&temp));
	} 
	else if (argc == 2) {
		char temp = *argv[1];
		set_max_indel_num(atoi(&temp));
		set_max_diff_num(atoi(&temp));
		set_print_detail(true);
		set_bit_mask(false);
		fprintf(stdout,"Output Mode : Detail\n");
		fprintf(stdout, "Bit mask: off\n");
		fprintf(stdout,"Max indel & diff : %d\n", atoi(&temp));
	}
	else {
		set_max_indel_num(3);
		set_max_diff_num(3);
		set_print_detail(true);
		set_bit_mask(false);
		fprintf(stdout,"Output Mode : Detail\n");
		fprintf(stdout, "Bit mask: off\n");
		fprintf(stdout,"Max indel & diff : 3\n");
	}
	set_fragment_length(101);
	set_key_number(fragment_length_/KEY_LENGTH);
	char file_ref [50];
	char file_input [50];
	char file_hash [50];
	char file_mask [50];
	char file_dist [50];
	sprintf(file_ref,  "%s", "result_ref_");
	sprintf(file_hash, "%s", "result_hash_");
	sprintf(file_input, "%s", "result_input");
	sprintf(file_mask, "%s", "result_mask_");
	set_select_cheapest(true);
	fprintf(stdout,"Mode : Alignment\n");
	sprintf(file_dist, "%s", "result_dist_cheap_");
	edit_normal(file_hash, file_mask, file_ref, file_dist, file_input);
	return 0;
}
