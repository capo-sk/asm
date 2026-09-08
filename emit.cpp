/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "emit.hpp"
#include "error.h"
extern "C" {
#include "opcodes.h"
}

Emitter::Emitter(NFile &a_nfile) : nfile(a_nfile) {
	nfile = a_nfile;
	pass = 1;
	flag = 0;
	loc = 0x0000;
	last_loc = 0xFFFF;
}

void Emitter::emit_byte(u8 value) {
	if (pass > 1) {
		if (flag == 0) {
			flag = 1;
			emit_word(loc);
			loc -= 2;
		}

		nfile.putc(value);
	}

	loc++;
	last_loc = loc;
}

void Emitter::emit_word(u16 value) {
	emit_byte(value & 0xFF);
	emit_byte((value >> 8) & 0xFF);
}

void Emitter::emit_bytes(u8 const *value, u16 count) {
	while (count > 0) {
		emit_byte(*value);
		++value;
		--count;
	}
}

void Emitter::emit_instruction(u8 index, u8 mode, u16 number, u8 size) {
	u8 opc_mode = opcodes[index].mode;
	u8 opc_code = opcodes[index].code;
	u8 opc_multi = opcodes[index].multi;
	u8 out_code;
	i16 distance;

	if (opc_mode == mode) {
		out_code = opc_code;
	} else {
		switch (opc_mode) {
			case rel_mode:
				if (mode == abs_mode || mode == zp_mode) {
					mode = rel_mode;
					out_code = opc_code;
					distance = (i16) (number - loc - 2);
					if (distance > 127 || distance < -128)
						abort_msg("Relative address too far");
					number = (u8) distance;
					size = 1;
				} else
					abort_msg("Invalid mode");
				break;
			case abs_ind_modes:
				if (mode == abs_mode)
					out_code = opc_code;
				else if (mode == ind_mode)
					out_code = opc_multi;
				else
					abort_msg("Invalid mode");
				break;
			case multimode:
			case multimode2:
			case multimode3:
				if (multimode_valid(opc_multi, mode))
					out_code = multimode_compose(index, mode);
				else
					abort_msg("Invalid mode");
				break;
			default:
				abort_msg("Internal error - no such mode");
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
			abort_msg("Internal error - invalid size");
	}
}

