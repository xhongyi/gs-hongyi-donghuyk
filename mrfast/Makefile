CC=gcc
CFLAGS = -c -O3 -pg
LDFLAGS = -lz -lm
SOURCES = baseFAST.c CommandLineParser.c Common.c HashTable.c MrFAST.c Output.c Reads.c RefGenome.c bit_mask.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = mrfast


all: $(SOURCES) $(EXECUTABLE)
	rm -rf *.o
		
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -pg $(OBJECTS) -o $@ $(LDFLAGS) 

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 
clean:
	rm -f *.o *~ #*
