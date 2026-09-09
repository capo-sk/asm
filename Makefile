CC := gcc
CFLAGS := -g -Wfatal-errors

CPP := g++
CPPFLAGS := $(CFLAGS)

LDFLAGS :=

#####

SRCS := assemble.cpp buffer.cpp emit.cpp error.c nfile.cpp opcodes.cpp parse.cpp srcfile.cpp symtable.cpp tkstream.cpp
OBJS := assemble.o buffer.o emit.o error.o nfile.o opcodes.o parse.o srcfile.o symtable.o tkstream.o

#####

all: asm

clean:
	rm *.o asm

deepclean:
	rm *.o asm Makefile.dep

asm: $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%o: %.cpp
	echo $(CPP) $(CPPFLAGS) -c $< -o $@

deps: $(SRCS)
	$(CPP) -MM $^ >Makefile.dep

-include Makefile.dep
