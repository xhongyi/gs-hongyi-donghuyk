/*
 * ED_test.cpp
 *
 *  Created on: Nov 12, 2011
 *      Author: mac
 */

#include "edit_distance.h"
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int main() {
	char test_read = (char*) malloc((FRAGMENT_LENGTH + 1)*sizeof(char));
	char ref_read = (char*) malloc((FRAGMENT_LENGTH + 1)*sizeof(char));
	set_max_indel_num(3);
	set_max_diff_num(3);
	allocatePath();
	cout << "started" << endl;
	strcpy(test_read, "CACGTTTTATTTTATTTTTTGAGATGGAGTCTCACTCTGTCACCCAGGCTGGAGTGTGGTGGCGCAATCTTTGCTCACTGCAACCTCCGCCTCCCGGGTTCAAGCGAT" );
	strcpy(ref_read,  "TTACTATATATATATTTTTTGAGATAGGGTCTCACTCTGTCACCCAGGCTGGAGTGCCCTGACATGATCTTGGCTCACTGCAACCTCCACCTCCCGGGTTCAAGCGAT" );
	ED_result result = editDistanceCal(test_read, ref_read, 1);
	if (result.correct) {
		cout << "Correct, passed the ED test" << endl;
		//cout << result.compare_result << endl;
	} else {
		cout << "Not correct" << endl;
	}
	/*
	 result
	 = editDistanceCal(
	 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABCDE",
	 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABCDEF");
	 if (result.correct) {
	 cout << "Correct, passed the ED test" << endl;
	 cout << result.compare_result << endl;
	 } else {
	 cout << "Not correct" << endl;
	 }
	 result
	 = editDistanceCal(
	 "CCCTAACCCTAACCCTAAACCCTAAACCCTAACCCTAACCCTAACCCTAACCCTAACCCCAACCCCAACCCCAACCCCAACCCCAACCCCAACCCTAACCCCTAACCC",

	 "CCCTAACCCTAAACCCTAAACCCTAACCCTAACCCTAACCCTAACCCTAACCCCAACCCCAACCCCAACCCCAACCCCAACCCCAACCCTAACCCCTAACCCTAACCC");
	 if (result.correct) {
	 cout << "Correct, passed the ED test" << endl;
	 cout << result.compare_result << endl;
	 } else {
	 cout << "Not correct" << endl;
	 }
	 */
	free(test_read);
	free(ref_read);
	return 0;
}
