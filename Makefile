CC=g++
CFLAGS = -pg -O3 
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = db_generator ref_db_generator

all: test_full
 
.c.o:
	$(CC) $(CFLAGS) $< -o $@

db_generator: common.h ref_db_generator.h hash_test.h ref_db_generator.cpp hash_loader.h hash_test.cpp hash_loader.cpp test_dhl.c
	$(CC) $(CFLAGS) common.cpp hash_generator.cpp hash_test.cpp ref_db_generator.cpp hash_loader.cpp test_dhl.c -o test_dhl

hash_generator: hash_generator.h hash_generator.cpp common.h
	$(CC) $(CFLAGS) hash_generator.cpp common.cpp test.cpp -o test

fragment_test:common.h common.cpp fragment_match.h fragment_match.cpp ref_db_generator.h ref_db_generator.cpp
	$(CC) $(CFLAGS) test_match.cpp fragment_match.cpp ref_db_generator.cpp hash_loader.cpp common.cpp -o fragment_match_test

ED_test: common.h common.cpp edit_distance.h edit_distance.cpp ED_test.cpp
	$(CC) $(CFLAGS) edit_distance.cpp common.cpp ED_test.cpp -o ed_test

distribution_test:common.h common.cpp fragment_match.h fragment_match.cpp ref_db_generator.h ref_db_generator.cpp  match_distribution.h match_distribution.cpp
	$(CC) $(CFLAGS) match_distribution.cpp fragment_match.cpp ref_db_generator.cpp hash_loader.cpp common.cpp test_distribution.c -o test_dist

test_full:common.h common.cpp fragment_match.h fragment_match.cpp ref_db_generator.h ref_db_generator.cpp edit_distribution.h hash_loader.cpp edit_distribution.cpp edit_distance.h edit_distance.cpp test_edit.c fragment_match.cpp edit_distribution_nofilter.cpp edit_distribution_nofilter.h
	$(CC) $(CFLAGS) edit_distribution_nofilter.cpp edit_distribution.cpp fragment_match.cpp ref_db_generator.cpp hash_loader.cpp common.cpp edit_distance.cpp test_edit.c -o test_full

test_correct:common.h correctness_checker.h common.cpp fragment_match.h fragment_match.cpp ref_db_generator.h ref_db_generator.cpp correctness_checker.cpp edit_distance.h edit_distance.cpp test_correctness.cpp
	$(CC) $(CFLAGS) correctness_checker.cpp fragment_match.cpp ref_db_generator.cpp hash_loader.cpp common.cpp edit_distance.cpp test_correctness.cpp -o test_correct

test_prefil:common.h common.cpp fragment_match.h fragment_match.cpp ref_db_generator.h ref_db_generator.cpp  prefil_distribution.cpp edit_distance.h edit_distance.cpp test_prefil.cpp
	$(CC) $(CFLAGS) prefil_distribution.cpp fragment_match.cpp ref_db_generator.cpp hash_loader.cpp common.cpp edit_distance.cpp test_prefil.cpp -o test_prefil

test_ref_loader: hash_loader.h hash_loader.cpp test_ref_loader.cpp
	$(CC) $(CFLAGS) hash_loader.cpp test_ref_loader.cpp -o test_ref_loader

test_shuffle: load_shuffled_read.h load_shuffled_read.cpp test_shuffle.cpp
	$(CC) $(CFLAGS) load_shuffled_read.cpp test_shuffle.cpp -o test_shuffle
