/*****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int string_size = 128;

int main()
{
 	FILE * pFileR;
 	FILE * pFileW;
 	char mystring [string_size];
 	char newstring [string_size+1];
//	int size1;
//	int size2;
	int size_ref_read = string_size;
	int i = 0;
	int j = 0;
	int flag_ref_end = 0;

 	pFileR = fopen ("ref" 	, "r");
 	pFileW = fopen ("result" , "w");

	while (flag_ref_end == 0) {
		if (i == 0) {
			size_ref_read = fread (mystring, 1, string_size, pFileR);
			i = size_ref_read;
// 			fprintf (stdout,"read size %i \n", size_ref_read);
		}
		switch (mystring[string_size-i]){
			case 'A': newstring[j]='A'; j=j+1; break;
			case 'C': newstring[j]='G'; j=j+1; break;
			case 'G': newstring[j]='C'; j=j+1; break;
			case 'T': newstring[j]='T'; j=j+1; break;
			case 'a': newstring[j]='A'; j=j+1; break;
			case 'g': newstring[j]='G'; j=j+1; break;
			case 'c': newstring[j]='C'; j=j+1; break;
			case 't': newstring[j]='T'; j=j+1; break;
			default: break;
		}
		i = i - 1;
// 			fprintf (stdout,"i: %i ",i);
		if (j == string_size) {
			newstring[j] = '\0';
 			fprintf (stdout,"%s \n", newstring);
			newstring[j] = '\n';
			fwrite (newstring, 1, j+1, pFileW);
			j = 0;
		} 
		else if ((size_ref_read < string_size)&&(i==0)) {
// 			fprintf (stdout,"Here \n");
			newstring[j] = '\0';
 			fprintf (stdout,"%s \n", newstring);
			newstring[j] = '\n';
			fwrite (newstring, 1, j+1, pFileW);
			j = 0;
			flag_ref_end = 1;
		}
	}

 	fclose (pFileR);
 	fclose (pFileW);
 	return 0;
}

// 			fprintf (stdout,"read size %i \n", size_ref_read);
// 	fprintf (stdout,"(j:%i, i:%i) ", j, i);
// 	if (pFile == NULL) perror ("Error opening file");
// 	else if ( fgets (mystring , 128 , pFile) != NULL ){
// 		puts (mystring);
// 	}
	//size1 = fread (mystring , 1, 128, pFile);
//	puts (mystring);
//	size2 = fread (mystring , 1, 128, pFile);
 //	puts (mystring);
 //	fprintf (stdout,"\n\n");
 //	fprintf (stdout,"size1 %i \n", size1);
 //	fprintf (stdout,"size2 %i \n", size2);
//
//
 //	fprintf (stdout,"test %1c \n", mystring[0]);
 //	fprintf (stdout,"test %1c \n", mystring[1]);
 //	fprintf (stdout,"test %1c \n", mystring[2]);
 //	fprintf (stdout,"test %1c \n", mystring[3]);
 //////	fprintf (stdout,"test %1c \n", mystring[4]);
