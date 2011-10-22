/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: mac
 */
#include <list>
#include <iostream>
#include "ref_db_generator.h"
#include "fragment_match.h"

using namespace std;

int main() {
	set_max_indel_num(1);
	set_max_diff_num(1);
	loadHash("testResult");
	list<match_result> result;
	string testee = "AGTAAAAAATAAATATGTTTAATTTGTGAACTGATTACCATCAGAATTGTACTGTTCTGTATCCACCAGCAATGTCTAGGAATGCCTGTTTCTCCACAA";
	result = searchFragment(testee);
	for (list<match_result>::iterator iter = result.begin(); iter != result.end(); iter++) {
		cout << "coordinate: " << iter->coordinate << endl;
		cout << "diff_num: " << iter->relevance << endl;
		cout << "Ref_sequence: " << getRefSeq(iter->coordinate, testee.size(), "ref_result_0") << endl;
	}
	return 0;
}
