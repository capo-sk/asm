/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "emit.h"
#include <stdio.h>
#include "loc.h"
#include "opcodes.h"
#include "error.h"
#define NDEBUG
#include "debug.h"

static u16 last_loc = 0xFFFF;
static u8 flag = 0;
static nfile_t *emit_nfile;

void emit_init(nfile_t *nfile) {
	emit_nfile = nfile;
}

void emit_byte(u8 value) {
	if (pass > 1) {
		if (flag == 0) {
			flag = 1;
			emit_word(location);
			location -= 2;
		}

		nfputc(emit_nfile, value);
	}

	++location;
	last_loc = location;
}

void emit_word(u16 value) {
	emit_byte(value & 0xFF);
	emit_byte((value >> 8) & 0xFF);
}

void emit_bytes(u8 const *value, u16 count) {
	while (count > 0) {
		emit_byte(*value);
		++value;
		--count;
	}
}

void emit_instruction(u8 index, u8 mode, u16 number, u8 size) {
	u8 opc_mode = opcodes[index].mode;
	u8 opc_code = opcodes[index].code;
	u8 opc_multi = opcodes[index].multi;
	u8 out_code;
	i16 distance;

	DV(location,u16)
	DV(opcodes[index].mnemonic,str)
	DV(opc_mode,u8)
	DV(mode,u8)
	DV(number,u16)
	DV(size,u8)

	if (opc_mode == mode) {
		out_code = opc_code;
	} else {
		switch (opc_mode) {
			case rel_mode:
				DV(mode,u8)
				if (mode == abs_mode || mode == zp_mode) {
					mode = rel_mode;
					out_code = opc_code;
					distance = (i16) (number - location - 2);
					DV(number,u16)
					DV(location,u16)
					DV(distance,i16)
					if (distance > 127 || distance < -128)
						error("Relative address too far");
					number = (u8) distance;
					DV(number,u16)
					size = 1;
				} else
					error("Invalid mode");
				break;
			case abs_ind_modes:
				if (mode == abs_mode)
					out_code = opc_code;
				else if (mode == ind_mode)
					out_code = opc_multi;
				else
					error("Invalid mode");
				break;
			case multimode:
			case multimode2:
			case multimode3:
				if (multimode_valid(opc_multi, mode))
					out_code = multimode_compose(index, mode);
				else
					error("Invalid mode");
				break;
			default:
				error("Internal error - no such mode");
		}
	}

	/* opcode */
	emit_byte(out_code);

	/* operand */
	switch (size) {
		case 0:
			break;
		case 1:
			emit_byte((u8) number);
			break;
		case 2:
			emit_word(number);
			break;
		default:
			error("Internal error - invalid size");
	}

	emit_next();
}

void emit_next(void) {
}
