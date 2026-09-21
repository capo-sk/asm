/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "srcfile.hpp"
#include <string>

#define _buf_line_max 255

class Buffer: public SrcStack {
private:
	int replay;
	bool eof;
	bool refill;
	char *ptr;
	char line[_buf_line_max + 1];

public:
	Buffer(std::string const &fname);

	int get_next();
	void pushback();
	void rewind();
	std::string getLineText();
	bool close_file();
	
	static const int eofmark = -1;
};

#endif
