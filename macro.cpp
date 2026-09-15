/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "macro.hpp"

MacroText::MacroText(std::string const &mname)
: SrcText(mname)
{
	current = text.begin();
}

MacroText::~MacroText()
{
}

void MacroText::InvokeAt(std::string const &fname, unsigned fline)
{
	filename = fname;
	fileline = fline;
}

void MacroText::AddLine(std::string const &tline)
{
	text.push_back(tline);
}

std::string MacroText::getFileLocation()
{
	return "&" + getName() + ":" + std::to_string(getLine())
		+ "@" + filename + ":" + std::to_string(fileline);
}

void MacroText::AdvanceLine()
{
	++current;
	SrcText::AdvanceLine();
}

void MacroText::Rewind()
{
	current = text.begin();
	SrcText::Rewind();
}
