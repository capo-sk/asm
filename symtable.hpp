/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP

#include <cstdint>
#include <map>
#include <string>
#include <ostream>

enum sym_type {
	sym_label,
	sym_var,
	sym_macro,
	sym_param,
	sym_anynum
};

struct sym_type_value {
	sym_type type;
	uint16_t value;
};

class SymbolTable: std::map<std::string, sym_type_value> {
private:
	void _add(char const *name, sym_type type, uint16_t value);

public:
	SymbolTable() = default;
	~SymbolTable() = default;

	void add_or_overwrite(char const *name, sym_type type, uint16_t value);
	void add_unique(char const *name, sym_type type, uint16_t value);
	bool get(char const *name, sym_type type, uint16_t &value);
	void dump(std::ostream &where, int format);
};

#endif
