CC=nvcc
CFLAGS = -g -arch=sm_20
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.cu
OBJECTS = $(SOURCES:.cu=.o)
EXECUTABLE = hash_test

all: edit_test full_test 
#cuda_test cuda_test multi_test

.cu.o:
	$(CC) $(CFLAGS) $< -o $@

hash_test:common.h common.cu hash_transfer.h hash_transfer.cu 
	$(CC) $(CFLAGS) test_hash.cu -o test_hash
cuda_test:common.h common.cu hash_loader.h hash_loader.cu hash_transfer.h hash_transfer.cu fragment_match.h fragment_match.cu edit_distance.h edit_distance.cu test_cuda.cu
	$(CC) $(CFLAGS) test_cuda.cu common.cu -o test_cuda
multi_test:common.h common.cu hash_loader.h hash_loader.cu hash_transfer.h hash_transfer.cu fragment_match.h fragment_match.cu edit_distance.h edit_distance.cu test_multi.cu
	$(CC) $(CFLAGS) test_multi.cu common.cu -o test_multi
full_test:common.h common.cu hash_loader.h hash_loader.cu hash_transfer.h hash_transfer.cu fragment_match.h fragment_match.cu edit_distance.h edit_distance.cu test_full.cu
	$(CC) $(CFLAGS) test_full.cu common.cu -o test_full
edit_test:common.h common.cu hash_transfer.h hash_transfer.cu 
	$(CC) $(CFLAGS) test_edit.cu -o test_edit
stream_test:common.h common.cu hash_loader.h hash_loader.cu hash_transfer.h hash_transfer.cu fragment_match.h fragment_match.cu edit_distance.h edit_distance.cu test_stream.cu
	$(CC) $(CFLAGS) test_stream.cu common.cu -lglut -o test_stream
