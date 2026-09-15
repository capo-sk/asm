/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "nfile.hpp"
#include <iostream>
#include <cstdarg>
#include <cstring>
#include "error.h"

NFile::NFile(int which)
: std::fstream()
{
	switch (which) {
		case 0:
			std::ios::rdbuf(std::cin.rdbuf());
			name = "<stdin>";
			break;
		case 1:
			std::ios::rdbuf(std::cout.rdbuf());
			name = "<stdout>";
			break;
		case 2:
			std::ios::rdbuf(std::cerr.rdbuf());
			name = "<stderr>";
			break;
		default:
			name = "<>";
	}
}

NFile::NFile(std::string const &a_name, std::ios_base::openmode mode)
: std::fstream(a_name, mode), name(a_name)
{
}

NFile::~NFile()
{
	close();
}

void NFile::printf(char const *fmt, ...) {
	char buffer[1024];
	int len;

	va_list params;

	va_start(params, fmt);
	len = vsnprintf(buffer, 1024, fmt, params);
	write(buffer, len);
	va_end(params);
}

void NFile::rewind() {
	seekg(0, beg);
}
