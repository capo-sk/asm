/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP

#include "types.h"
#include "nfile.hpp"
#include <map>
#include <string>

enum sym_type {
	sym_label,
	sym_var,
	sym_macro,
	sym_param,
	sym_anynum
};

struct sym_type_value {
	sym_type type;
	u16 value;
};

class SymbolTable: std::map<std::string, sym_type_value> {
private:
	void _add(char const *name, sym_type type, u16 value);

public:
	SymbolTable();
	~SymbolTable();

	void add(char const *name, sym_type type, u16 value);
	void addnew(char const *name, sym_type type, u16 value);
	bool get(char const *name, sym_type type, u16 &value);
	void dump(NFile &where, int format);
};

#endif
