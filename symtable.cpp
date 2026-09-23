/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "symtable.hpp"
#include <cstring>
#include "error.h"
#include "util.hpp"

SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
}

void SymbolTable::_add(char const *name, sym_type type, uint16_t value) {
	insert({name, {type, value}});
}

void SymbolTable::add_unique(char const *name, sym_type type, uint16_t value) {
	if (count(name) != 0)
		abort_fmt("Duplicate symbol %s", name);
	else
		_add(name, type, value);
}

void SymbolTable::add_or_overwrite(char const *name, sym_type type, uint16_t value) {
	auto it = find(name);

	if (it == end())
		_add(name, type, value);
	else
		if (it->second.type == type)
			it->second.value = value;
		else
			abort_fmt("Symbol %s type mismatch", name);
}

bool SymbolTable::get(char const *name, sym_type type, uint16_t &value) {
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
			abort_fmt("Symbol %s type mismatch", name);
	}
}

void SymbolTable::dump(std::ostream &where, int format) {
	static char const *decode[] = { "label", "variable", "macro", "param", "any" };

	if (format == 0)
		where << "Symbols\n";

	for (auto it = begin(); it != end(); ++it)
		if (format == 0)  /* human consumption */
			where << sfmt("%s(%s) = %u $%04X\n", it->first.c_str(), decode[it->second.type], it->second.value, it->second.value);
		else  /* VICE monitor format */
			where << sfmt("al C:%X .%s\n", it->second.value, it->first.c_str());
}
