/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "macro.hpp"
#include <cstring>

using namespace std;

MacroText::MacroText(string const &mname)
: SrcText(mname), values(NULL)
{
	current = text.begin();
}

MacroText::~MacroText()
{
}

void MacroText::Invoke(SrcText const &callfrom, list<string> const &pvalues)
{
	filename = callfrom.getName();
	fileline = callfrom.getLine();
	values = &pvalues;
}

void MacroText::AddLine(string const &tline)
{
	text.push_back(tline);
}

string MacroText::getLocation() const
{
	return "&" + getName() + ":" + to_string(getLine())
		+ "@" + filename + ":" + to_string(fileline);
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

void MacroText::AddParam(string const &param)
{
	params.push_back(param);
}

string _replace(string const &base, list<string> const &params, list<string> const &values)
{
	return base;
}

bool MacroText::getline(char *buffer, unsigned max)
{
	if (current != text.end()) {
		string tx = _replace(current->data(), params, *values);
		strncpy(buffer, tx.c_str(), max - 1);
		buffer[max - 1] = 0;
		return true;
	} else
		return false;
}

void MacroTable::add(MacroText &macro)
{
	insert({macro.getName(), macro});
}
