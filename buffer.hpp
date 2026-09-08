/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#ifndef BUFFER_H
#define BUFFER_H

#include "srcfile.hpp"

#define _buf_line_max 255

class Buffer {
private:
	SrcFileList &slist;
	int replay;
	int eof;
	char *ptr;
	char line[_buf_line_max + 1];

public:
	Buffer(SrcFileList &srcl);
	int get_next();
	void rewind_1();
	void rewind();
	void AdvanceLine();
	char const *getFilename();
	unsigned getLinenum();
	
	static const int eofmark = -1;
};

#endif
