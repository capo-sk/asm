CC=gcc
CFLAGS=-g

CPP=g++
CPPFLAGS=-g -std=c++20 -Wfatal-errors

LDFLAGS=

#####

SRCS := assemble.cpp buffer.cpp emit.cpp error.cpp opcodes.cpp parse.cpp srcfile.cpp symtable.cpp tkstream.cpp source.cpp macro.cpp srcstack.cpp
OBJS := assemble.o buffer.o emit.o error.o opcodes.o parse.o srcfile.o symtable.o tkstream.o source.o macro.o srcstack.o version.o

#####

all: uasm

clean:
	rm *.o uasm version.c tests/helper/make_ref

deepclean: clean
	rm Makefile.dep

uasm: $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

version.c: $(SRCS)
	echo 'const char version_string[] = "'`date -u +%Y%m%d_%H%M`'";' >$@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

deps: $(SRCS)
	$(CPP) $(CPPFLAGS) -MM $^ >Makefile.dep

-include Makefile.dep

tests/helper/make_ref: tests/helper/make_ref.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<

test: uasm tests/helper/make_ref
	cd tests && ./all_tests.sh
