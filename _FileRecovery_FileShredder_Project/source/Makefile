
CC=gcc  
CFLAGS=-c -g
LDFLAGS=

SOURCES=main.c scan_main.c scan_super.c scan_gd.c scan_inode.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=runScan

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ 

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -f core \#* *.bak *~ *.o $(EXECUTABLE)  */*~


