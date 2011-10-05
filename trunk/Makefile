CC=gcc
CFLAGS = -c -O3 -pg
LDFLAGS = -lz -lm 
SOURCES = ref_db_generator.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ref_db_generator

all: $(SOURCES) $(EXECUTABLE)
	rm -rf *.o
		
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -pg $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
