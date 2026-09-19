/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#include "buffer.hpp"
#include <iostream>

using namespace std;

int Buffer::get_next() {
	if (eof)
		return eofmark;

	if (replay > 0xff) {  // after rewind_1
		replay &= 0xff;
	} else {
		if (refill) {
			if (!slist.getline(line, sizeof(line))) { // EOF
				cerr << line << "\n";
				eof = true;
				return eofmark;
			}
			ptr = &line[0];
			refill = false;
			cerr << line << "\n";
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

void Buffer::rewind_1() {
	replay |= 0x100;
}

void Buffer::rewind() {
	slist.getCurrent().Rewind();
	replay = 0;
	line[0] = 0;
	ptr = &line[0];
	eof = false;
}

Buffer::Buffer(SrcFileList &srcl) : slist(srcl) {
	replay = 0;
	line[0] = 0;
	ptr = &line[0];
	eof = false;
	refill = true;
}

void Buffer::AdvanceLine() {
	slist.AdvanceLine();
}

bool Buffer::close_file() {
	replay = 0;
	line[0] = 0;
	ptr = &line[0];
	eof = false;
	return slist.Pop();
}

void Buffer::new_file(char const *name) {
	if (!slist.isPresent(name))  // only process file if first encounter
		slist.AddOnce(new SrcFile(name));
}

void Buffer::reset() {
	slist.Reset();
}

std::string Buffer::getLocation()
{
	return slist.getLocation();
}

string Buffer::getLineText()
{
	return string(line);
}

SrcText &Buffer::getCurrent()
{
	return slist.getCurrent();
}

void Buffer::new_source(SrcText *source)
{
	slist.Add(source);
}
