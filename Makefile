CC=cc
CFLAGS=

all: asm

asm: assemble.c parse.c tkstream.c buffer.c token.c debug.c opcodes.c loc.c error.c emit.c symtable.c nfile.c
	$(CC) $(CFLAGS) -o $@ $^

