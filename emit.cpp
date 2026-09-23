/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "emit.hpp"
#include "opcodes.hpp"

Emitter::Emitter(std::ostream &a_nfile)
: nfile(a_nfile)
{
	pass = 1;
	flag = 0;
	loc = 0x0000;
	last_loc = 0xFFFF;
	err = NULL;
}

void Emitter::error_provider(ErrorProvider *ep)
{
	err = ep;
}

void Emitter::emit_byte(uint8_t value)
{
	if (pass > 1) {
		if (flag == 0) {
			flag = 1;
			emit_word(loc);
			loc -= 2;
		}

		nfile.put(value);
	}

	loc++;
	last_loc = loc;
}

void Emitter::emit_word(uint16_t value)
{
	emit_byte(value & 0xFF);
	emit_byte((value >> 8) & 0xFF);
}

void Emitter::emit_bytes(uint8_t const *value, uint16_t count)
{
	while (count > 0) {
		emit_byte(*value);
		++value;
		--count;
	}
}

void Emitter::emit_instruction(uint8_t index, uint8_t mode, uint16_t number, uint8_t size)
{
	uint8_t opc_mode = opcodes[index].mode;
	uint8_t opc_code = opcodes[index].code;
	uint16_t opc_multi = opcodes[index].multi;
	uint8_t out_code;
	int16_t distance;
	uint8_t upgraded_mode;

	if (opc_mode == mode) {
		out_code = opc_code;
	} else {
		switch (opc_mode) {
			case rel_mode:
				if (mode == abs_mode || mode == zp_mode) {
					mode = rel_mode;
					out_code = opc_code;
					distance = (int16_t) (number - loc - 2);
					if (distance > 127 || distance < -128)
						err->error("Relative address too far");
					number = (uint8_t) distance;
					size = 1;
				} else
					err->error("Invalid mode");
				break;
			case abs_ind_modes:
				if (mode == abs_mode)
					out_code = opc_code;
				else if (mode == ind_mode)
					out_code = opc_multi;
				else
					err->error("Invalid mode");
				break;
			case multimode:
			case multimode2:
			case multimode3:
				if (multimode_valid(opc_multi, mode))
					upgraded_mode = mode;
				else {
					switch (mode) {
						case zp_mode:
							upgraded_mode = abs_mode;
							break;
						case zpx_mode:
							upgraded_mode = absx_mode;
							break;
						case zpy_mode:
							upgraded_mode = absy_mode;
							break;
						default:
							upgraded_mode = mode;
					}
					size++;
				}
				if (multimode_valid(opc_multi, upgraded_mode))
					out_code = multimode_compose(index, upgraded_mode);
				else
					err->error("Invalid mode");
				break;
			default:
				err->error("Internal error - no such mode");
		}
	}

	/* opcode */
	emit_byte(out_code);

	/* operand */
	switch (size) {
		case 0:
			break;
		case 1:
			emit_byte((uint8_t) number);
			break;
		case 2:
			emit_word(number);
			break;
		default:
			err->error("Internal error - invalid size");
	}
}

