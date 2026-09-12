/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "symtable.hpp"
#include <cstring>
#include "error.h"

SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
}

void SymbolTable::_add(char const *name, sym_type type, u16 value) {
	insert({name, {type, value}});
}

void SymbolTable::addnew(char const *name, sym_type type, u16 value) {
	if (count(name) != 0)
		abort_fmt("Duplicate symbol %s", name);
	else
		_add(name, type, value);
}

void SymbolTable::add(char const *name, sym_type type, u16 value) {
	auto it = find(name);

	if (it == end())
		_add(name, type, value);
	else
		if (it->second.type == type)
			it->second.value = value;
		else
			abort_fmt("Symbol %s type mismatch", name);
}

bool SymbolTable::get(char const *name, sym_type type, u16 &value) {
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

void SymbolTable::dump(NFile &where, int format) {
	static char const *decode[] = { "label", "variable", "macro", "param", "any" };

	if (format == 0)
		where.printf("Symbols\n");

	for (auto it = begin(); it != end(); ++it)
		if (format == 0)  /* human consumption */
			where.printf("%s(%s) = %u $%04X\n", it->first.c_str(), decode[it->second.type], it->second.value, it->second.value);
		else  /* VICE monitor format */
			where.printf("al C:%X .%s\n", it->second.value, it->first.c_str());
}
