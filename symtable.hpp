/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP

#include "error.hpp"
#include <cstdint>
#include <map>
#include <ostream>
#include <string>

using std::string;

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

class SymbolTable: public ErrorConsumer, std::map<string, sym_type_value> {
private:
	void _add(string const &name, sym_type type, uint16_t value);

public:
	SymbolTable() = default;
	~SymbolTable() = default;

	void add_or_overwrite(string const &name, sym_type type, uint16_t value);
	void add_unique(string const &name, sym_type type, uint16_t value);
	bool get(string const &name, sym_type type, uint16_t &value);
	void dump(std::ostream &where, int format);
};

#endif
