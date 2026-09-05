#ifndef EMIT_H
#define EMIT_H

#include "types.h"

void emit_byte(u8 value);
void emit_word(u16 value);
void emit_bytes(u8 const *value, u16 count);
void emit_instruction(u8 index, u8 mode, u16 number, u8 size);
void emit_next(void);

#endif
