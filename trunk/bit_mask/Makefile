CC=g++
CFLAGS = -O3 #-mcmodel=medium
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = db_generator ref_db_generator bm_generator bm_analyzer

all: test_normal hash_generator hash_analyzer bm_generator# bm_analyzer
 
.c.o:
	$(CC) $(CFLAGS) $< -o $@

db_generator: common.h ref_db_generator.h hash_test.h ref_db_generator.cpp hash_loader.h hash_test.cpp hash_loader.cpp test_dhl.c
	$(CC) $(CFLAGS) common.cpp hash_generator.cpp hash_test.cpp ref_db_generator.cpp hash_loader.cpp test_dhl.c -o test_dhl

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

test_GPU:common.h common.cpp fragment_match.h fragment_match.cpp ref_db_generator.h ref_db_generator.cpp  prefil_distribution.cpp edit_distance.h edit_distance.cpp test_prefil.cpp
	$(CC) $(CFLAGS) GPU_distribution.cpp fragment_match.cpp ref_db_generator.cpp hash_loader.cpp common.cpp edit_distance.cpp test_GPU.cpp -o test_GPU

test_ref_loader: hash_loader.h hash_loader.cpp test_ref_loader.cpp
	$(CC) $(CFLAGS) hash_loader.cpp test_ref_loader.cpp -o test_ref_loader

test_shuffle: load_shuffled_read.h load_shuffled_read.cpp test_shuffle.cpp
	$(CC) $(CFLAGS) load_shuffled_read.cpp test_shuffle.cpp -o test_shuffle

hash_generator: hash_generator.h hash_generator.cpp common.h ref_db_generator.h ref_db_generator.cpp hash_loader.h hash_loader.cpp test_hash.c
	$(CC) $(CFLAGS) hash_generator.cpp common.cpp ref_db_generator.cpp hash_loader.cpp test_hash.c -o gen_hash

test_normal:bit_mask.h bit_mask.cpp common.h common.cpp fragment_match.h fragment_match.cpp ref_db_generator.h ref_db_generator.cpp edit_normal.h hash_loader.cpp edit_normal.cpp edit_distance.h edit_distance.cpp fragment_match.cpp 
	$(CC) $(CFLAGS) bit_mask.cpp edit_normal.cpp fragment_match.cpp ref_db_generator.cpp hash_loader.cpp common.cpp edit_distance.cpp test_normal.cpp -o test_align

bm_generator: bit_mask_generator.h bit_mask_generator.cpp bit_mask_main.cpp
	$(CC) $(CFLAGS) bit_mask_generator.cpp bit_mask_main.cpp -o bm_generator

bm_analyzer: bm_analyzer_main.cpp bit_mask_analyzer.h bit_mask_analyzer.cpp hash_loader.h hash_loader.cpp
	$(CC) $(CFLAGS) hash_loader.cpp bm_analyzer_main.cpp bit_mask_analyzer.cpp -o bm_analyzer
	 
hash_analyzer: hash_analyzer_main.cpp hash_analyzer.h hash_analyzer.cpp common.h hash_loader.h hash_loader.cpp
	$(CC) $(CFLAGS) hash_loader.cpp hash_analyzer_main.cpp hash_analyzer.cpp -o hash_analyzer
	
friend_hash_generator: friend_hash_generator.h friend_hash_generator.cpp friend_hash_generator_main.cpp
	$(CC) $(CFLAGS) friend_hash_generator.cpp friend_hash_generator_main.cpp -o friend_hash_generator
