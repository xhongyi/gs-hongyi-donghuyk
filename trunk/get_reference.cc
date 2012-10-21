#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

void printDNA(ifstream& DNA_file, int chromo_number, int location, int length) {
	DNA_file.seekg(0, ios_base::beg);
	int temp_chromo = 0;
	string temp_string;
	//get to the chromosome
	while (1) {
		getline(DNA_file, temp_string);
		if (temp_string[0] == '>') {
			temp_chromo++;
			cout << temp_string << endl;
			if (temp_chromo == chromo_number)
				break;
		}
	}

	//get to the location;
	int temp_loc = 0;
	while (temp_loc < location) {
		DNA_file >> temp_string;
		temp_loc += temp_string.size();
	}

	//concatenate the string
	string out_string;
	out_string = temp_string.substr(
			location + temp_string.size() - temp_loc - 1,
			temp_loc + 1 - location);
	while (out_string.size() < length) {
		DNA_file >> temp_string;
		out_string += temp_string;
	}

	out_string = out_string.substr(0, length);
	cout << "reference DNA is: " << out_string << endl;
}

int main(int argc, const char* argv[]) {
	ifstream DNA_file;
	if (argc != 2) {
		cerr << "Please also put the reference file name" << endl;
		exit(1);
	}
	DNA_file.open(argv[1]);

	if (!DNA_file) {
		cerr << "DNA file can't be opened" << endl;
		exit(1);
	}
	int chromo_number;
	int location;
	int length;
	while (1) {
		cout << "please input chromo number: " << endl;
		cin >> chromo_number;
		cout << "please input location: " << endl;
		cin >> location;
		cout << "please input length: " << endl;
		cin >> length;

		printDNA(DNA_file, chromo_number, location, length);
	}
	return 0;
}
