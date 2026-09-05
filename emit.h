/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef EMIT_H
#define EMIT_H

#include <stdio.h>
#include "types.h"

void emit_init(FILE *binary);
void emit_byte(u8 value);
void emit_word(u16 value);
void emit_bytes(u8 const *value, u16 count);
void emit_instruction(u8 index, u8 mode, u16 number, u8 size);
void emit_next(void);

#endif
