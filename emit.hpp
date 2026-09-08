/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef EMIT_H
#define EMIT_H

#include "types.h"
#include "nfile.hpp"

class Emitter {
private:
	NFile &nfile;
	unsigned pass;
	int flag;
	u16 loc;
	u16 last_loc;

	void emit_next(void);

public:
	Emitter(NFile &a_nfile);
	u16 get_loc() { return loc; }
	void set_loc(u16 a_loc) { loc = a_loc; }
	void set_pass(unsigned a_pass) { pass = a_pass; }
	void emit_byte(u8 value);
	void emit_word(u16 value);
	void emit_bytes(u8 const *value, u16 count);
	void emit_instruction(u8 index, u8 mode, u16 number, u8 size);
};

#endif
