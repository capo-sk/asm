/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP

#include "types.h"
#include "nfile.hpp"

enum SymbolType {
	sym_label,
	sym_var,
	sym_any
};

struct SymbolNode {
        char *name;
        SymbolType type;
        u16 value;
        SymbolNode *next;
};

class SymbolTable {
private:
	SymbolNode *first, *last;

	SymbolNode *_find(char const *name);
	void _add(char const *name, SymbolType type, u16 value);

public:
	SymbolTable();
	~SymbolTable();

	void add(char const *name, SymbolType type, u16 value);
	void addnew(char const *name, SymbolType type, u16 value);
	int get(char const *name, SymbolType type, u16 &value);
	void dump(NFile &where, int format);
};

#endif
