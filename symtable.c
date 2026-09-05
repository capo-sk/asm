/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "symtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#define DEBUG
#include "debug.h"

typedef struct symnode {
	char *name;
	sym_type type;
	u16 value;
	struct symnode *next;
} symnode;

static symnode *symlist_first = null;
static symnode *symlist_last = null;

static symnode *symlist_find(char const *name) {
	symnode *p = symlist_first;
	while (p) {
		if (strcmp(p->name, name) == 0)
			break;
		p = p->next;
	}
	return p;
}

static char *dup_str(char const *s) {
	char *d;
	unsigned size;

	size = strlen(s) + 1;
	d = malloc(size);
	memcpy(d, s, size);

	return d;
}

static void symlist_add(char const *name, sym_type type, u16 value) {
	symnode *p;

	p = malloc(sizeof(symnode));
	p->name = dup_str(name);
	p->type = type;
	p->value = value;
	p->next = null;

	if (symlist_last == null)
		symlist_first = p;
	else
		symlist_last->next = p;
	symlist_last = p;
}

void sym_new(char const *name, sym_type type, u16 value) {
	if (symlist_find(name) != null)
		error_str("Duplicate symbol %s", name);
	else
		symlist_add(name, type, value);
}

void sym_add(char const *name, sym_type type, u16 value) {
	symnode *p;

	p = symlist_find(name);
	if (p == null)
		symlist_add(name, type, value);
	else
		if (p->type == type)
			p->value = value;
		else
			error_str("Symbol %s type mismatch", name);
}

int sym_get(char const *name, sym_type type, u16 *value) {
	symnode *p;

	p = symlist_find(name);
	if (p == null) {
		return 0;
	} else {
		if (type == p->type || type == sym_any) {
			*value = p->value;
			return 1;
		} else
			error_str("Symbol %s type mismatch", name);
	}
}

void sym_dump(void) {
	symnode *p;

	fprintf(stderr, "Symbols\n");
	for (p = symlist_first; p != null; p = p->next)
		fprintf(stderr, "%s(%u) = %u $%04X\n", p->name, p->type, p->value, p->value);
	fprintf(stderr, "\n");
}
