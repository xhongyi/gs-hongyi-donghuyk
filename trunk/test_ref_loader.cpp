#include <iostream>
#include <string>
#include "hash_loader.h"

using namespace std;

int main() {
	char ref_file[20];
	strcpy(ref_file, "toy_ref_0");
	string ref;
	refLoader (ref, ref_file);
	cout << ref << endl;
	return 0;
}
