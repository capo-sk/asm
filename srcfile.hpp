/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SRCFILE_HPP
#define SRCFILE_HPP

#include "source.hpp"
#include <string>

class SrcFile: public SrcText {
private:
	std::ifstream *nfile;

public:
	SrcFile(std::string const &fname);
	virtual ~SrcFile();

	virtual void rewind();

	virtual bool getline(std::string &buffer);
};

#endif
