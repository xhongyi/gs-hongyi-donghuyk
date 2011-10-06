/*
 * commom.h
 *
 *  Created on: Oct 5, 2011
 *      Author: mac
 */

#ifndef COMMOM_H_
#define COMMOM_H_

#define KEY_LENGTH 12
//How many files hash table should break up to. Must be pow of 4
#define HASH_FILE_POW 2
#define HASH_FILE_NUM (1 << (2 * HASH_FILE_POW) )

char **hashfile;

#endif /* COMMOM_H_ */
