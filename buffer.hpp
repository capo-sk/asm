/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "srcfile.hpp"

#define _buf_line_max 255

class Buffer {
private:
	SrcFileList &slist;
	int replay;
	bool eof;
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
	bool close_file();
	void new_file(char const *name);
	void reset();
	
	static const int eofmark = -1;
};

#endif
