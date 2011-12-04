/*
 * ED_test.cpp
 *
 *  Created on: Nov 12, 2011
 *      Author: mac
 */

#include "edit_distance.h"
#include <iostream>

using namespace std;

int main() {
	char test_read[READ_LENGTH];
	char ref_read[READ_LENGTH];
	set_max_indel_num(5);
	set_max_diff_num(5);
	allocatePath();
	cout << "started" << endl;
	strcpy(test_read, "ACCTGGTACAAAAGGTCAGTTGATGTTGGCTGCTACTTTTTTGTTGTTTTGAGACAGGGTCTCACTCTGTCACCCAGGCTGGAGTGCAGAGGCCTGATCATGACTCAC");
	strcpy(ref_read,  "GCACCTGGTAAAAGGTCAGTTGATGTTAGCTGCTACTTTTTTGTTGTTTTGAGACAGGGTCTCACTCTGTCACCCAGGCTGGAGTGCAGAGGCCTGATCATGACTCAC");
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
	return 0;
}
