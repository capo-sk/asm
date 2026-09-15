/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "source.hpp"

SrcText::SrcText(std::string const &tname)
: textname(tname), linenum(1)
{
}

SrcText::~SrcText()
{
}

std::string SrcText::getLocation()
{
	return textname + ":" + std::to_string(linenum);
}

void SrcText::AdvanceLine()
{
	++linenum;
}

void SrcText::Rewind()
{
	linenum = 1;
}

unsigned SrcText::getLine()
{
	return linenum;
}

std::string const &SrcText::getName()
{
	return textname;
}
