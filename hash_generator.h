/*
 * hash_generator.h
 *
 *  Created on: Oct 5, 2011
 *      Author: mac
 */

#ifndef HASH_GENERATOR_H_
#define HASH_GENERATOR_H_

#include "common.h"
#include <string>
#include <deque>

using namespace std;

//Hash table stored here
deque<deque<deque<int> > > hash_table;

/*
 * Function: Generates hash table. Store it in hash_table
 * Input: reference file name
 */
void hashTableGenerator(string ref_name);

/*
 * Function: Dump all the memory stuff to off-line files. It will
 * store multiple files determined by HASH_FILE_NUM.
 * Input: The starting name of the hash file. All the hash_table
 * file will be stored as "hash_name_x", where x is the value computed
 * by int hashIdx(char* key)
 */
//void hashFileWriter(string hash_name);

/*
 * Function: Given a key sequence, compute the hash table value
 * Input: Key_sequence
 * Return: hash_value
 */
int hashVal(string key);

/*
 * Function: Given a key sequence, compute the index value of which
 * hash table to address to.
 * Input: Key_sequence
 * Return: hash_index
 */
int hashIdx(string key);

#include "hash_generator.cpp"
#endif /* HASH_GENERATOR_H_ */
