CC      = gcc
CFLAGS  = -Wall -Wstrict-prototypes
SOURCES = net.c
INCLUDE = error.h net.h
OBJECTS = $(SOURCES:.c=.o)
BIN     = libquicknet.so

all:
	for i in $(SOURCES); do \
		$(CC) $(CFLAGS) -c -fPIC $$i -o obj/$$i.o; \
	done

	gcc -shared -Wl,-soname,$(BIN).1 -o $(BIN) obj/*.o
	
clean:
	rm -rf obj/*
	rm $(BIN)
