CC := gcc
CFLAGS := -g -Wfatal-errors

CPP := g++
CPPFLAGS := $(CFLAGS)

LDFLAGS :=

#####

SRCS := assemble.cpp buffer.cpp emit.cpp error.c opcodes.cpp parse.cpp srcfile.cpp symtable.cpp tkstream.cpp source.cpp macro.cpp util.cpp
OBJS := assemble.o buffer.o emit.o error.o opcodes.o parse.o srcfile.o symtable.o tkstream.o source.o macro.o util.o version.o

#####

all: uasm

clean:
	rm *.o uasm version.c

deepclean:
	rm *.o uasm Makefile.dep

uasm: $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

version.c: $(SRCS)
	echo 'const char version_string[] = "'`date -u +%Y%m%d_%H%M`'";' >$@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%o: %.cpp
	echo $(CPP) $(CPPFLAGS) -c $< -o $@

deps: $(SRCS)
	$(CPP) -MM $^ >Makefile.dep

-include Makefile.dep

test: uasm
	cd tests && ./all_tests.sh
