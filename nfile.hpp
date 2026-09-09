/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef NFILE_HPP
#define NFILE_HPP

#include <cstdio>

class NFile {
private:
	FILE *file;
	char *name;

	[[noreturn]] void _fail();

public:
	NFile(char const *name, char const *mode = "r");
	NFile(int stdfile);
	NFile();
	~NFile();
	char const *GetName() { return name; };
	void putc(unsigned char c);
	unsigned getline(char *output, unsigned max);
	void rewind();
	void printf(char const *fmt, ...);
};

#endif
