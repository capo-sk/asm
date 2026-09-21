/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#include "buffer.hpp"
#include <iostream>

using namespace std;

int Buffer::get_next()
{
	if (eof)
		return eofmark;

	if (replay > 0xff) {  // after rewind_1
		replay &= 0xff;
	} else {
		if (refill) {
			if (!getline(line, sizeof(line))) { // EOF
//				cerr << line << "\n";
				eof = true;
				return eofmark;
			}
			ptr = &line[0];
			refill = false;
//			cerr << line << "\n";
		}

		if (*ptr != 0) { // not end of line yet
			replay = *(unsigned char *)ptr++;
		} else {  // reached end of line
			replay = 10;
			refill = true;
		}
	}

	return replay;
}

void Buffer::pushback()
{
	replay |= 0x100;
}

void Buffer::rewind() {
	get_current().rewind();
	replay = 0;
	line[0] = 0;
	ptr = &line[0];
	eof = false;
}

Buffer::Buffer(string const &fname)
: SrcStack(fname)
{
	replay = 0;
	line[0] = 0;
	ptr = &line[0];
	eof = false;
	refill = true;
}

/*
Buffer::Buffer(SrcStack &srcl) : slist(srcl) {
	replay = 0;
	line[0] = 0;
	ptr = &line[0];
	eof = false;
	refill = true;
}
*/

bool Buffer::close_file() {
	replay = 0;
	line[0] = 0;
	ptr = &line[0];
	eof = false;
	return Pop();
}

string Buffer::get_line_text()
{
	return string(line);
}

