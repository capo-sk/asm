/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "macro.hpp"
#include "error.hpp"

#include <format>

/*MacroDefinition::MacroDefinition(string const &mname)
{
}

MacroDefinition::~MacroDefinition()
{
}*/

/*void MacroText::invoke(SrcText const &callfrom, list<string> const &pvalues)
{
	filename = callfrom.get_name();
	fileline = callfrom.get_linenum();
	values = &pvalues;
	current = text.begin();
}*/

void MacroDefinition::add_line(string const &tline)
{
	text.push_back(tline);
}

string MacroInvocation::get_location() const
{
	return "&" + get_name() + ":" + std::to_string(get_linenum())
		+ "@" + filename + ":" + std::to_string(fileline);
}

void MacroInvocation::advance_line()
{
	++current;
	SrcText::advance_line();
}

void MacroInvocation::rewind()
{
	current = definition->text.begin();
	SrcText::rewind();
}

static string _replace_1(string const &word, std::list<string> const &params, std::list<string> const &values)
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

static string _replace(string const &base, std::list<string> const &params, std::list<string> const &values)
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

bool MacroInvocation::getline(string &buffer)
{
	if (current != definition->text.end()) {
		buffer = _replace(current->data(), definition->params, values);
		advance_line();
		return true;
	} else {
		buffer.clear();
		return false;
	}
}

void MacroDefinition::add_param(string const &param)
{
	params.push_back(param);
}

unsigned MacroDefinition::get_param_count() const
{
	return params.size();
}

MacroInvocation::MacroInvocation(string const &name, MacroDefinition const *def, SrcText const &callfrom)
: SrcText(name), definition(def)
{
	filename = callfrom.get_name();
	fileline = callfrom.get_linenum();
	current = definition->text.begin();
}

//void MacroDefinition::new_invocation()
//{
//	values.clear();
//	rewind();
//}

void MacroInvocation::add_value(string const &value)
{
	values.push_back(value);
}

unsigned MacroInvocation::get_value_count() const
{
	return values.size();
}

void MacroTable::add(string const &name, std::unique_ptr<MacroDefinition> macrop)
{
	auto [it, result] = try_emplace(name, std::move(macrop));

	if (!result)
		error(std::format("Duplicate macro {}", name));
}
