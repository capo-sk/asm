/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* OPCODES */

#ifndef OPCODES_H
#define OPCODES_H

#include "types.h"

#define indx_bit  0x01
#define zp_bit    0x02
#define imm_bit   0x04
#define abs_bit   0x08
#define indy_bit  0x10
#define zpx_bit   0x20
#define absy_bit  0x40
#define absx_bit  0x80
#define all_bits  0xFF
#define rega_bit  imm_bit
#define zpy_bit   indy_bit
#define imm2_bit  imm_bit

extern const u8 multimode_select[];
extern const u8 multimode2_select[];
extern const u8 multimode3_select[];

#define indx_mode 0
#define zp_mode   1
#define imm_mode  2
#define abs_mode  3
#define indy_mode 4
#define zpx_mode  5
#define absy_mode 6
#define absx_mode 7
#define impl_rega 8
#define zpy_mode  9
#define impl_mode 10
#define rel_mode  11
#define ind_mode  12
#define multimode 16
#define multimode2 17
#define multimode3 18
#define abs_ind_modes 19


typedef struct {
	char mnemonic[4];
	u8 mode;
	u8 code;
	u8 multi;
} opcode;

extern const opcode opcodes[];
extern const u8 num_opcodes;

typedef struct {
	char mnemonic[8];
} pseudo;

extern const pseudo pseudos[];
extern const u8 num_pseudos;
extern const u8 pseudo_macro;

/* registers */
extern const char *cpu_registers[];
extern const u8 num_registers;

u8 multimode_opcode(u8 base_code, u8 mode);
u8 multimode2_opcode(u8 base_code, u8 mode);
u8 multimode3_opcode(u8 base_code, u8 mode);
bool multimode_valid(u8 multi_bits, u8 mode);
u8 multimode_compose(u8 index, u8 mode);

#endif
