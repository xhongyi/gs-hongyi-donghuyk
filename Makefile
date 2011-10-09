CC=g++
CFLAGS = -g
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ref_db_generator

all: db_generator hash_generator

.c.o:
	$(CC) $(CFLAGS) $< -o $@

db_generator: common.h ref_db_generator.h hash_test.h ref_db_generator.c hash_loader.h hash_test.c hash_loader.c test_dhl.c
	$(CC) $(CFLAGS) common.cpp hash_generator.cpp hash_test.c ref_db_generator.c hash_loader.c test_dhl.c -o test_dhl

hash_generator: hash_generator.h hash_generator.cpp common.h
	$(CC) $(CFLAGS) hash_generator.cpp common.cpp test.cpp -o test
