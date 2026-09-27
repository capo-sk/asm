/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "symtable.hpp"
#include "error.hpp"

void SymbolTable::_add(string const &name, sym_type type, uint16_t value)
{
	insert({name, {type, value}});
}

void SymbolTable::add_unique(string const &name, sym_type type, uint16_t value)
{
	if (count(name) != 0)
		error(format("Duplicate symbol {}", name));
	else
		_add(name, type, value);
}

void SymbolTable::add_or_overwrite(string const &name, sym_type type, uint16_t value)
{
	auto it = find(name);

	if (it == end())
		_add(name, type, value);
	else
		if (it->second.type == type)
			it->second.value = value;
		else
			error(format("Symbol {} type mismatch", name));
}

bool SymbolTable::get(string const &name, sym_type type, uint16_t &value)
{
	auto it = find(name);

	if (it == end())
		return false;
	else {
		if (type == it->second.type || type == sym_anynum) {
			value = it->second.value;
			return true;
		} else
			error(format("Symbol {} type mismatch", name));
	}
}

void SymbolTable::dump(std::ostream &where, int fmt)
{
	static char const *decode[] = { "label", "variable", "any" };

	if (fmt == 0)
		where << "Symbols\n";

	for (auto it = begin(); it != end(); ++it)
		if (fmt == 0) { // human consumption
			string stype;
			switch (it->second.type) {
				case sym_label:
					stype = "label";
					break;
				case sym_var:
					stype = "variable";
					break;
				default:
					stype = "unknown";
			}
			where << format("{0}({2}) = {1} ${1:04X}\n", it->first, it->second.value, stype);
		} else  // VICE monitor format
			where << format("al C:{1:X} .{0}\n", it->first, it->second.value);
}
