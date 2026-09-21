/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "source.hpp"

SrcText::SrcText(std::string const &tname)
: textname(tname), linenum(0)
{
}

std::string SrcText::get_location() const
{
	return textname + ":" + std::to_string(linenum);
}

void SrcText::advance_line()
{
	++linenum;
}

void SrcText::rewind()
{
	linenum = 1;
}

unsigned SrcText::get_line() const
{
	return linenum;
}

std::string const &SrcText::get_name() const
{
	return textname;
}

