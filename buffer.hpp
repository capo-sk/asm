/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "common.hpp"
#include "srcstack.hpp"

#define _buf_line_max 255

class Buffer: public SrcStack {
private:
	int replay;
	bool eof;
	bool refill;
	string sline;
	size_t sidx;

public:
	Buffer(string const &fname);

	int get_next();
	void pushback();
	void rewind();
	string get_line_text() const;
	bool close_file();
	
	static const int eofmark = -1;
};

#endif
