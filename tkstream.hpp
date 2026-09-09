/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef TKSTREAM_HPP
#define TKSTREAM_HPP

#include "buffer.hpp"
#include "types.h"

enum token_context {
	line_start,
	line_middle,
	macro_header1,
	macro_header2,
	end_of_file,
	err_too_long,
	err_invalid_chr
};

enum token_type {
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
};

#define VALUE_SIZE 64
#define MAX_TOKEN_LENGTH (VALUE_SIZE - 1)

struct Token {
	token_type type;
	char value[VALUE_SIZE];
};

class TokenStream {
private:
	Buffer &buf;
	token_context ctx;
	Token latest;
	bool reuse;

	int _get_next_token(Token *tk);

public:
	TokenStream(Buffer &in_buf);

	int read(Token &tk);
	void rewind_1();
	void rewind();
	void reset();
	void AdvanceLine();
	char const *getFilename();
	unsigned getLinenum();
	void nested_file(char const *name);
};

#endif
