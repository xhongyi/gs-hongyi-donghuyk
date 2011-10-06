CC=g++
CFLAGS = -g
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ref_db_generator

all: db_generator hash_generator

.c.o:
	$(CC) $(CFLAGS) $< -o $@

db_generator: ref_db_generator.c
	$(CC) $(CFLAGS) ref_db_generator.c -o ref_db_generator

hash_generator: hash_generator.h hash_generator.cpp common.h
	$(CC) $(CFLAGS) test.cpp hash_generator.cpp -o test