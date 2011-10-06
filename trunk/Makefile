CC=g++
CFLAGS = -c -O3 -pg
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ref_db_generator

all: db_generator
		
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -pg $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

db_generator: ref_db_generator.c
	$(CC) $(CFLAGS) ref_db_generator.c -o $ref_db_generator
