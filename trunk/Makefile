CC=g++
CFLAGS = -g
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = db_generator ref_db_generator

all: db_generator distribution_test hash_generator fragment_test distribution_test

.c.o:
	$(CC) $(CFLAGS) $< -o $@

db_generator: common.h ref_db_generator.h hash_test.h ref_db_generator.cpp hash_loader.h hash_test.cpp hash_loader.cpp test_dhl.c
	$(CC) $(CFLAGS) common.cpp hash_generator.cpp hash_test.cpp ref_db_generator.cpp hash_loader.cpp test_dhl.c -o test_dhl

hash_generator: hash_generator.h hash_generator.cpp common.h
	$(CC) $(CFLAGS) hash_generator.cpp common.cpp test.cpp -o test

fragment_test:common.h common.cpp fragment_match.h fragment_match.cpp ref_db_generator.h ref_db_generator.cpp
	$(CC) $(CFLAGS) test_match.cpp fragment_match.cpp ref_db_generator.cpp hash_loader.cpp common.cpp -o fragment_match_test

distribution_test:common.h common.cpp fragment_match.h fragment_match.cpp ref_db_generator.h ref_db_generator.cpp  match_distribution.cpp
	$(CC) $(CFLAGS) match_distribution.cpp fragment_match.cpp ref_db_generator.cpp hash_loader.cpp common.cpp test_distribution.c -o test_dist
