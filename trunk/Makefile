CC=gcc
CFLAGS = -c -O3 -pg
LDFLAGS = -static -lz -lm 
SOURCES = ref_alignment.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ref_alignment

all: $(SOURCES) $(EXECUTABLE)
	rm -rf *.o
		
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -pg $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
