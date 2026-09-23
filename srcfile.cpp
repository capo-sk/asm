/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "srcfile.hpp"
#include "error.h"
#include <fstream>

using namespace std;

// class SrcFile

SrcFile::SrcFile(std::string const &name)
: SrcText(name)
{
	nfile = new ifstream(name);
	if (!*nfile)
		abort_sys(name.c_str());
}

SrcFile::~SrcFile()
{
	if (nfile) {
		nfile->close();
		delete nfile;
	}
}

void SrcFile::rewind()
{
	// clear eof/err and go back to beginning of the file
	nfile->clear();
	nfile->seekg(0, ios::beg);

	SrcText::rewind();
}

bool SrcFile::getline(string &buffer)
{
	std::getline(*nfile, buffer);
	if (buffer.length() == 0 && nfile->eof())
		return false;
	else {
		advance_line();
		return true;
	}
}

