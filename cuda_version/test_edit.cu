/*
 * test_search.cpp
 *
 *  Created on: Oct 19, 2011
 *	  Author: mac
 */
#include <iostream>
#include "universal_header.h"

using namespace std;


#include "hash_test.cu"


__global__ void cuda_editDistanceCal (char * dev_test_read, char * dev_ref_read, ED_result * result) {
	ED_path path[20];
	/*
	 * The trick is in the third agrument. If I pick the forward and backward breaking position at the same position,
	 * it going all correct.
	 * However, if I changed it like some starts at key 0, some key 1, some key 2 like what I did here,
	 * the warp will break down. Actually, it will break down in a way that the key 0 ones will go into 1 warp
	 * key 1 ones will go into 1 warp and key 2 ones will go into 1 warp.
	 * Feel free to tweak the %number.
	 */
	result[threadIdx.x] = editDistanceCal(dev_test_read, dev_ref_read, 2, path, 6, 5, 5);
}

int test_cuda(void) {

	char test_read[READ_LENGTH];
	strcpy(
			test_read,
//			"GGGTGGTAGGTGCAGAGACGGGAGGGGCAGAGCCGCAGGCACAGCCAAGAGGGCTGAAGAAATGGTAGAACGGAGCAGCTGGTGATGTGTGGGCCCACCGGCCCCAGG");
			"CACGTTTTATTTTATTTTTTGAGATGGAGTCTCACTCTGTCACCCAGGCTGGAGTGTGGTGGCGCAATCTTTGCTCACTGCAACCTCCGCCTCCCGGGTTCAAGCGAT");

	//Getting the sort key.

	char ref_read[READ_LENGTH];
	strcpy(
			ref_read,
			//"GGGTGGTAGGTGCAGAGACGGGAGGGGCAGAGCCGCAGGCACAGCCAAGAGGGCTGAAGAAATGGTAGAACGGAGCAGCTGGTGATGTGTGGGCCCACCGGCCCCAGG");
			"TTACTATATATATATTTTTTGAGATAGGGTCTCACTCTGTCACCCAGGCTGGAGTGCCCTGACATGATCTTGGCTCACTGCAACCTCCACCTCCCGGGTTCAAGCGAT");

	ED_result result[1];

	char * dev_test_read;
	char * dev_ref_read;

	ED_result * dev_result;

	cudaMalloc( (void**) &dev_test_read, READ_LENGTH * sizeof(char) );
	cudaMalloc( (void**) &dev_ref_read, READ_LENGTH * sizeof(char) );

	cudaMemcpy(dev_test_read, test_read, READ_LENGTH * sizeof(char), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_ref_read, ref_read, READ_LENGTH * sizeof(char), cudaMemcpyHostToDevice);

	cudaMalloc( (void**) &dev_result, 1 * sizeof(ED_result) );

	cuda_editDistanceCal <<<1, 1>>> (dev_test_read, dev_ref_read, dev_result);

	cudaMemcpy(result, dev_result, 1 * sizeof(ED_result), cudaMemcpyDeviceToHost);

	for (int i = 0; i < 1; i++) {
		cout << "i: " << i << endl;
		if (result[i].correct) {
			cout << "correct! " << endl;
			cout << "result[" << i << "].diff_num: " << result[i].diff_num << endl;
			for (int j = 0; j < result[i].diff_num; i++) {
			 	switch (result[i].error[j].diff) {
			 	case MISMATCH:
			 		cout << "Mismatch at " << result[i].error[j].location << endl;
					cout << "Error: " << result[i].error[j].diff_char << endl;
			 		break;
				case INSERTION:
			 		cout << "Insertion at " << result[i].error[j].location << endl;
			 		cout << "Error: " << result[i].error[j].diff_char << endl;
			 		break;
			 	case DELETION:
			 		cout << "Deletion at " << result[i].error[j].location << endl;
			 		cout << "Error: " << result[i].error[j].diff_char << endl;
			 		break;
			 	}
			}
		}
		else
			cout << "Incorrect!!!" << endl;
	}

	cudaFree(dev_test_read);
	cudaFree(dev_ref_read);
	cudaFree(dev_result);
	return 0;
}

int main() {
	test_cuda();
	return 0;
}
