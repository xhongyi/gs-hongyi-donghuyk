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
	set_max_diff_num(2);
	loadHash("toy_hash_0");
	list<match_result> result;
//	string testee = "ACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCCTAACCCTA";
	string testee = "ACCCTAACCCTAACCCTAACCCAAACCCTAACCCTAACCCAAACCCTAACCCTAACCCAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCCTAACCCTA";
	result = searchFragment(testee);
	for (list<match_result>::iterator iter = result.begin(); iter != result.end(); iter++) {
		cout << "coordinate: " << iter->coordinate << endl;
		cout << "diff_num: " << iter->relevance << endl;
		cout << "Ref_sequence: " << getRefSeq(iter->coordinate, testee.size(), "toy_ref_0") << endl;
		cout << "# of possible location: " << result.size() << endl;
	}
	return 0;
}
