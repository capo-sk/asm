/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "macro.hpp"
#include <cstring>

using namespace std;

MacroText::MacroText(string const &mname)
: SrcText(mname)
{
	current = text.begin();
}

MacroText::~MacroText()
{
}

/*void MacroText::invoke(SrcText const &callfrom, list<string> const &pvalues)
{
	filename = callfrom.get_name();
	fileline = callfrom.get_linenum();
	values = &pvalues;
	current = text.begin();
}*/

void MacroText::add_line(string const &tline)
{
	text.push_back(tline);
}

string MacroText::get_location() const
{
	return "&" + get_name() + ":" + to_string(get_linenum())
		+ "@" + filename + ":" + to_string(fileline);
}

void MacroText::advance_line()
{
	++current;
	SrcText::advance_line();
}

void MacroText::rewind()
{
	current = text.begin();
	SrcText::rewind();
}

static string _replace_1(string const &word, list<string> const &params, list<string> const &values)
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

static string _replace(string const &base, list<string> const &params, list<string> const &values)
{
	char const *current, *word;
	bool out;
	string result;

	current = base.c_str();
	word = current;
	out = true;
	unsigned char c;  // unsigned because isalnum is undefined for negative char
	while ((c = *current) != 0) {
		if (!(isalnum(c) || c == '_')) {  // word (macro param) can contain letters, digits, underscore
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

bool MacroText::getline(string &buffer)
{
	if (current != text.end()) {
		buffer = _replace(current->data(), params, values);
		advance_line();
		return true;
	} else {
		buffer.clear();
		return false;
	}
}

void MacroText::add_param(string const &param)
{
	params.push_back(param);
}

unsigned MacroText::get_param_count() const
{
	return params.size();
}

void MacroText::new_invocation()
{
	values.clear();
	rewind();
}

void MacroText::add_value(std::string const &value)
{
	values.push_back(value);
}

unsigned MacroText::get_value_count() const
{
	return values.size();
}

void MacroTable::add(MacroText &macro)
{
	insert({macro.get_name(), macro});
}
