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
	current = text.begin();
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

string _replace_1(string const &word, list<string> const &params, list<string> const &values)
{
	auto pi = params.cbegin();
	auto vi = values.cbegin();
	while (pi != params.cend()) {
		if (*pi == word) {
			return *vi;
		}
		++pi;
		++vi;
	}
	return word;
}

string _replace(string const &base, list<string> const &params, list<string> const &values)
{
	char const *current, *word;
	bool out;
	string result;

	current = base.c_str();
	word = current;
	out = true;
	char c;
	while ((c = *current) != 0) {
		if (c == ' ' || c == '9' || c == '!' || c == '$' || c == '+' || c == '-') {
			if (!out) { // end of word
				out = true;
				string sword(word, current - word);
				result.append(_replace_1(sword, params, values));
				word = current;
			}
		} else {
			if (out) { // end of non-word
				out = false;
				result.append(word, current - word);
				word = current;
			}
		}

		++current;
	}
	if (out)
		result.append(word, current - word);
	else {
		string sword(word, current - word);
		result.append(_replace_1(sword, params, values));
	}

	return result;
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
