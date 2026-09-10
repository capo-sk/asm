/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* OPCODES */

#ifndef OPCODES_HPP
#define OPCODES_HPP

#include "types.h"

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

#define indx_bit  (1u << indx_mode)
#define zp_bit    (1u << zp_mode)
#define imm_bit   (1u << imm_mode)
#define abs_bit   (1u << abs_mode)
#define indy_bit  (1u << indy_mode)
#define zpx_bit   (1u << zpx_mode)
#define absy_bit  (1u << absy_mode)
#define absx_bit  (1u << absx_mode)
#define all_bits  0xffu
#define rega_bit  (1u << impl_rega)
#define zpy_bit   (1u << zpy_mode)
#define imm2_bit  imm_bit

extern const u8 multimode_select[];
extern const u8 multimode2_select[];
extern const u8 multimode3_select[];

typedef struct {
	char mnemonic[4];
	u8 mode;
	u8 code;
	u16 multi;
} opcode;

extern const opcode opcodes[];
extern const u8 num_opcodes;

typedef struct {
	char const *mnemonic;
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
bool multimode_valid(u16 multi_bits, u8 mode);
u8 multimode_compose(u8 index, u8 mode);

#endif
