/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "symtable.hpp"
#include <cstring>
#include <format>

void SymbolTable::_add(string const &name, sym_type type, uint16_t value) {
	insert({name, {type, value}});
}

void SymbolTable::add_unique(string const &name, sym_type type, uint16_t value) {
	if (count(name) != 0)
		error(std::format("Duplicate symbol {}", name));
	else
		_add(name, type, value);
}

void SymbolTable::add_or_overwrite(string const &name, sym_type type, uint16_t value) {
	auto it = find(name);

	if (it == end())
		_add(name, type, value);
	else
		if (it->second.type == type)
			it->second.value = value;
		else
			error(std::format("Symbol {} type mismatch", name));
}

bool SymbolTable::get(string const &name, sym_type type, uint16_t &value) {
	auto it = find(name);

	if (it == end())
		return false;
	else {
		if ( (type == it->second.type) || 
			( type == sym_anynum && (it->second.type == sym_label || it->second.type == sym_var) )
			) {
			value = it->second.value;
			return true;
		} else
			error(std::format("Symbol {} type mismatch", name));
	}
}

void SymbolTable::dump(std::ostream &where, int format) {
	static char const *decode[] = { "label", "variable", "macro", "param", "any" };

	if (format == 0)
		where << "Symbols\n";

	for (auto it = begin(); it != end(); ++it)
		if (format == 0)  /* human consumption */
			where << std::format("{0}({2}) = {1} ${1:04X}\n", it->first, it->second.value, decode[it->second.type]);
		else  /* VICE monitor format */
			where << std::format("al C:{1:X} .{0}\n", it->first, it->second.value);
}
