/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "symtable.hpp"
#include <cstring>
#include "error.h"

SymbolTable::SymbolTable() {
	first = NULL;
	last = NULL;
}

SymbolTable::~SymbolTable() {
	SymbolNode *p, *n;

	for (p = first; p != NULL; p = n) {
		n = p->next;
		delete p;
	}
}

SymbolNode *SymbolTable::_find(char const *name) {
	SymbolNode *p;

	for (p = first; p != NULL; p = p->next)
		if (strcmp(p->name, name) == 0)
			break;

	return p;
}

void SymbolTable::_add(char const *name, SymbolType type, u16 value) {
	SymbolNode *p;

	p = new SymbolNode;
	p->name = strdup(name);
	p->type = type;
	p->value = value;
	p->next = NULL;

	if (last == NULL)
		first = p;
	else
		last->next = p;
	last = p;
}

void SymbolTable::addnew(char const *name, SymbolType type, u16 value) {
	if (_find(name) != NULL)
		abort_fmt("Duplicate symbol %s", name);
	else
		_add(name, type, value);
}

void SymbolTable::add(char const *name, SymbolType type, u16 value) {
	SymbolNode *p;

	p = _find(name);
	if (p == NULL)
		_add(name, type, value);
	else
		if (p->type == type)
			p->value = value;
		else
			abort_fmt("Symbol %s type mismatch", name);
}

int SymbolTable::get(char const *name, SymbolType type, u16 &value) {
	SymbolNode *p;

	p = _find(name);
	if (p == NULL)
		return 0;
	else {
		if (type == p->type || type == sym_any) {
			value = p->value;
			return 1;
		} else
			abort_fmt("Symbol %s type mismatch", name);
	}
}

void SymbolTable::dump(NFile &where, int format) {
	SymbolNode *p;

	if (format == 0)
		where.printf("Symbols\n");

	for (p = first; p != NULL; p = p->next)
		if (format == 0)  /* human consumption */
			where.printf("%s(%u) = %u $%04X\n", p->name, p->type, p->value, p->value);
		else  /* VICE monitor format */
			where.printf("al C:%X .%s\n", p->value, p->name);
}
