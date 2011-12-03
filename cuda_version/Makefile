CC=nvcc
CFLAGS = -g -lglut
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.cu
OBJECTS = $(SOURCES:.cu=.o)
EXECUTABLE = hash_test

all: hash_test

.cu.o:
	$(CC) $(CFLAGS) $< -o $@

hash_test:common.h common.cu fragment_match.h fragment_match.cu 
	$(CC) $(CFLAGS) common.cu hash_loader.cu fragment_match.cu test_hash.cu -o test_hash