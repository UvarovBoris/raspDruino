CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lwiringPi
SOURCES=dht.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=dht

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
