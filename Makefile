CC=cc
CFLAGS=-g -Wfatal-errors

all: asm

asm: assemble.cpp nfile.cpp srcfile.cpp error.c emit.cpp
	$(CC) $(CFLAGS) -o $@ $^

