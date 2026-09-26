/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#include "buffer.hpp"

using namespace std;

int Buffer::get_next()
{
	if (eof)
		return eofmark;

	if (replay > 0xff) {  // after rewind_1
		replay &= 0xff;
	} else {
		if (refill) {
			if (!getline(sline)) { // EOF
				eof = true;
				return eofmark;
			}
			sidx = 0;
			refill = false;
		}

		if (sidx < sline.length()) {  // not end of line yet
			replay = sline[sidx++];
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
	sline.clear();
	sidx = 0;
	eof = false;
	refill = true;
}

Buffer::Buffer(string const &fname)
: SrcStack(fname), replay(0), eof(false), refill(true)
{
//	replay = 0;
//	sline.clear();
//	eof = false;
//	refill = true;
}

bool Buffer::close_file() {
	replay = 0;
	sline.clear();
	sidx = 0;
	eof = false;
	refill = true;
	return pop();
}

string Buffer::get_line_text() const
{
	return sline;
}

