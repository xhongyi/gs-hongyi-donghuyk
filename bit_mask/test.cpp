/*
 * test.cpp
 *
 *  Created on: Oct 6, 2011
 *      Author: mac
 */
#include <iostream>
#include "hash_generator.h"

using namespace std;

int main() {
	hashTableGenerator("ref_result_0");
	hashFileWriter("testResult");
	return 0;
}
