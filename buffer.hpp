/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "srcstack.hpp"
#include <string>

#define _buf_line_max 255

class Buffer: public SrcStack {
private:
	int replay;
	bool eof;
	bool refill;
	std::string sline;
	size_t sidx;

public:
	Buffer(std::string const &fname);

	int get_next();
	void pushback();
	void rewind();
	std::string get_line_text() const;
	bool close_file();
	
	static const int eofmark = -1;
};

#endif
