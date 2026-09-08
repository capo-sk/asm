/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* OPCODES */

#include "opcodes.h"

const u8 multimode_select[10] =
	{ 0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x08, 0x14 };
const u8 multimode2_select[10] =
	{ 0x00, 0x04, 0x00, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x08, 0x14 };
const u8 multimode3_select[10] =
	{ 0x00, 0x04, 0x00, 0x0C, 0x10, 0x1C, 0x18, 0x1C, 0x08, 0x14 };
const u8 multimode_bits[10] =
	{ indx_bit, zp_bit, imm_bit, abs_bit, indy_bit, zpx_bit,
	  absy_bit, absx_bit, rega_bit, zpy_bit };

bool multimode_valid(u8 multi_bits, u8 mode) {
	if (mode > 9)
		return false;

	if ((multi_bits & multimode_bits[mode]) != 0)
		return true;
	else
		return false;
}

u8 multimode_opcode(u8 base_code, u8 mode) {
	return base_code | multimode_select[mode];
}
u8 multimode2_opcode(u8 base_code, u8 mode) {
	return base_code | multimode2_select[mode];
}
u8 multimode3_opcode(u8 base_code, u8 mode) {
	return base_code | multimode3_select[mode];
}

u8 multimode_compose(u8 index, u8 mode) {
	u8 opc_mode = opcodes[index].mode;
	u8 opc_base = opcodes[index].code;

	switch (opc_mode) {
		case multimode:
			return multimode_opcode(opc_base, mode);
		case multimode2:
			return multimode2_opcode(opc_base, mode);
		case multimode3:
			return multimode3_opcode(opc_base, mode);
		default:
			return 0xFF;
	}
}


const opcode opcodes[] = {
	{ "ADC", multimode, 0x61, all_bits },
	{ "AND", multimode, 0x21, all_bits },
	{ "ASL", multimode2, 0x02, zp_bit | rega_bit | abs_bit | zpx_bit | absx_bit },
	{ "BCC", rel_mode, 0x90 },
	{ "BCS", rel_mode, 0xB0 },
	{ "BEQ", rel_mode, 0xF0 },
	{ "BIT", multimode, 0x20, zp_bit | abs_bit },
	{ "BMI", rel_mode, 0x30 },
	{ "BNE", rel_mode, 0xD0 },
	{ "BPL", rel_mode, 0x10 },
	{ "BRK", impl_mode, 0x00 },
	{ "BVC", rel_mode, 0x50 },
	{ "BVS", rel_mode, 0x70 },
	{ "CLC", impl_mode, 0x18 },
	{ "CLD", impl_mode, 0xD8 },
	{ "CLI", impl_mode, 0x58 },
	{ "CLV", impl_mode, 0xB8 },
	{ "CMP", multimode, 0xC1, all_bits },
	{ "CPX", multimode2, 0xE0, imm2_bit | zp_bit | abs_bit },
	{ "CPY", multimode2, 0xC0, imm2_bit | zp_bit | abs_bit },
	{ "DEC", multimode, 0xC2, zp_bit | abs_bit | zpx_bit | absx_bit },
	{ "DEX", impl_mode, 0xCA },
	{ "DEY", impl_mode, 0x88 },
	{ "EOR", multimode, 0x41, all_bits },
	{ "INC", multimode, 0xE2, zp_bit | abs_bit | zpx_bit | absx_bit },
	{ "INX", impl_mode, 0xE8 },
	{ "INY", impl_mode, 0xC8 },
	{ "JMP", abs_ind_modes, 0x4C, 0x6C },
	{ "JSR", abs_mode, 0x20 },
	{ "LDA", multimode, 0xA1, all_bits },
	{ "LDX", multimode3, 0xA2, imm2_bit | zp_bit | abs_bit | zpy_bit | absy_bit },
	{ "LDY", multimode2, 0xA0, imm2_bit | zp_bit | abs_bit | zpx_bit | absx_bit },
	{ "LSR", multimode2, 0x42, zp_bit | rega_bit | abs_bit | zpx_bit | absx_bit },
	{ "NOP", impl_mode, 0xEA },
	{ "ORA", multimode, 0x01, all_bits },
	{ "PHA", impl_mode, 0x48 },
	{ "PHP", impl_mode, 0x08 },
	{ "PLA", impl_mode, 0x68 },
	{ "PLP", impl_mode, 0x28 },
	{ "ROL", multimode2, 0x22, zp_bit | rega_bit | abs_bit | zpx_bit | absx_bit },
	{ "ROR", multimode2, 0x62, zp_bit | rega_bit | abs_bit | zpx_bit | absx_bit },
	{ "RTI", impl_mode, 0x40 },
	{ "RTS", impl_mode, 0x60 },
	{ "SBC", multimode, 0xE1, all_bits },
	{ "SEC", impl_mode, 0x38 },
	{ "SED", impl_mode, 0xF8 },
	{ "SED", impl_mode, 0xF8 },
	{ "SEI", impl_mode, 0x78 },
	{ "STA", multimode, 0x81, all_bits & (~imm_bit) },
	{ "STX", multimode3, 0x82, zp_bit | abs_bit | zpy_bit },
	{ "STY", multimode, 0x80, zp_bit | abs_bit | zpx_bit },
	{ "TAX", impl_mode, 0xAA },
	{ "TAY", impl_mode, 0xA8 },
	{ "TSX", impl_mode, 0xBA },
	{ "TXA", impl_mode, 0x8A },
	{ "TXS", impl_mode, 0x9A },
	{ "TYA", impl_mode, 0x98 },
};
const u8 num_opcodes = sizeof(opcodes) / sizeof(opcode);

const pseudo pseudos[] = {
	{ "MACRO" },
	{ "ENDM" },
	{ ".BYTE" },
	{ ".WORD" },
	{ ".INCLUDE" }
};
const u8 num_pseudos = sizeof(pseudos) / sizeof(pseudo);
const u8 pseudo_macro = 0;


const char *cpu_registers[] = { "A", "X", "Y" };
const u8 num_registers = 3;
