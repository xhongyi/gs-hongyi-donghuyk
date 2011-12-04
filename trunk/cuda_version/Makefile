CC=nvcc
CFLAGS = -g
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.cu
OBJECTS = $(SOURCES:.cu=.o)
EXECUTABLE = hash_test

all: hash_test

.cu.o:
	$(CC) $(CFLAGS) $< -o $@

hash_test:common.h common.cu hash_transfer.h hash_transfer.cu 
	$(CC) $(CFLAGS) common.cu hash_loader.cu hash_transfer.cu test_hash.cu -o test_hash
