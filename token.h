/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* TOKENS */

#ifndef TOKEN_H
#define TOKEN_H

#include "buffer.h"
#include "types.h"

typedef enum {
	line_start,
	line_middle,
	macro_header1,
	macro_header2,
	end_of_file,
	err_too_long,
	err_invalid_chr
} token_context;

typedef enum {
	invalid = 0,
	/* standalone characters with their value 1-127 */
	endline = 0x80,
	actual_opcode,
	pseudo_opcode,
	cpu_register,
	macro_def,
	macro_ref,
	macro_par,
	label_def,
	var_def,
	symbol_ref,
	literal_dec,
	literal_hex,
	literal_chr,
	literal_str
} token_type;

#define VALUE_SIZE 64
#define MAX_TOKEN_LENGTH (VALUE_SIZE - 1)

typedef struct {
	token_type type;
	char value[VALUE_SIZE];
} token;

int get_next_token(buffer_ref *in_buf, token_context *ctx, token *tk);

#endif
