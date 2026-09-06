/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SYM_TABLE
#define SYM_TABLE

#include "types.h"
#include "nfile.h"

typedef enum {
	sym_label,
	sym_var,
	sym_any
} sym_type;

void sym_new(char const *name, sym_type type, u16 value);
void sym_add(char const *name, sym_type type, u16 value);
int sym_get(char const *name, sym_type type, u16 *value);
void sym_dump(nfile_t *where, int format);

#endif
