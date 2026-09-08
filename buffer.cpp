/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#include "buffer.hpp"

int Buffer::get_next() {
	if (eof)
		return eofmark;

	if (replay > 0xff) {  // after rewind_1
		replay &= 0xff;
	} else {
		if (*ptr == 0) {  // need new line
			if (slist.GetNFile()->getline(line, sizeof(line)) == 0) {  // EOF
				eof = 1;
				return eofmark;
			}
			ptr = &line[0];
		}
		replay = *(unsigned char *)ptr++;
	}

	return replay;
}

void Buffer::rewind_1() {
	replay |= 0x100;
}

void Buffer::rewind() {
	slist.GetNFile()->rewind();
	replay = eofmark;
	line[0] = 0;
	ptr = &line[0];
	eof = 0;
}

Buffer::Buffer(SrcFileList &srcl) : slist(srcl) {
	replay = eofmark;
	line[0] = 0;
	ptr = &line[0];
	eof = 0;
}

void Buffer::AdvanceLine() {
	slist.AdvanceLine();
}

char const *Buffer::getFilename() {
	return slist.getFilename();
}

unsigned Buffer::getLinenum() {
	return slist.getLinenum();
}

void Buffer::close_file() {
	slist.Pop();
}
