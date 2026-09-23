/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* OPCODES */

#ifndef OPCODES_HPP
#define OPCODES_HPP

#include <cstdint>

#define indx_mode 0	// OPC (byte,X)
#define zp_mode   1	// OPC byte
#define imm_mode  2	// OPC #byte
#define abs_mode  3	// OPC word
#define indy_mode 4	// OPC (byte),Y
#define zpx_mode  5	// OPC byte,X
#define absy_mode 6	// OPC word,Y
#define absx_mode 7	// OPC word,X
#define impl_rega 8	// OPC A
#define impl_mode 9	// OPC
#define zpy_mode  10	// OPC byte,Y
#define rel_mode  11	// BOP label
#define ind_mode  12	// OPC (word)
#define multimode 16	// LDA etc
#define multimode2 17	// LDY etc
#define multimode3 18	// LDX etc
#define abs_ind_modes 19 // JMP

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
#define impl_bit  (1u << impl_mode)
#define rega_bits (rega_bit | impl_bit)
#define zpy_bit   (1u << zpy_mode)

extern const uint8_t multimode_select[];
extern const uint8_t multimode2_select[];
extern const uint8_t multimode3_select[];

typedef struct {
	char mnemonic[4];
	uint8_t mode;
	uint8_t code;
	uint16_t multi;
} opcode;

extern const opcode opcodes[];
extern const uint8_t num_opcodes;

typedef struct {
	char const *mnemonic;
} pseudo;

extern const pseudo pseudos[];
extern const uint8_t num_pseudos;
extern const uint8_t pseudo_macro;

/* registers */
extern const char *cpu_registers[];
extern const uint8_t num_registers;

uint8_t multimode_opcode(uint8_t base_code, uint8_t mode);
uint8_t multimode2_opcode(uint8_t base_code, uint8_t mode);
uint8_t multimode3_opcode(uint8_t base_code, uint8_t mode);
bool multimode_valid(uint16_t multi_bits, uint8_t mode);
uint8_t multimode_compose(uint8_t index, uint8_t mode);

#endif
