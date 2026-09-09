CC=gcc
CFLAGS=-g -Wfatal-errors

CPP=g++
CPPFLAGS=-g -Wfatal-errors

all: asm

clean:
	rm *.o asm

%.o: %.c
	$(CC) $(CFLAGS) -c $^

%o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^

asm: assemble.o nfile.o srcfile.o error.o emit.o tkstream.o buffer.o opcodes.o symtable.o parse.o
	$(CPP) -o $@ $^

