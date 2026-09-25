/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef TKSTREAM_HPP
#define TKSTREAM_HPP

#include "buffer.hpp"
#include <string>

enum token_mode {
	assembly,
	macro,
	words
};

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
	literal_bin,
	literal_chr,
	literal_str,
	macro_line = 0xc0,
	word = 0xd0,
	skip = 0xf0
};

struct Token {
	token_type type;
	union {
		std::string value;
		unsigned number;
	};

	Token();
	~Token() {};
	Token &operator=(Token const &tk);

	void clear();
};

class TokenStream {
private:
	Buffer &buf;
	token_mode mode;
	token_context ctx;
	Token latest;
	bool reuse;

	int _get_next_token_multimode(Token &tk);
	int _get_next_token(Token &tk);
	int _get_next_macro_line(Token &tk);
	int _get_next_word(Token &tk);

public:
	TokenStream(Buffer &in_buf);

	int read(Token &tk);
	void pushback();
	void rewind();
	void reset();
	void set_mode(token_mode a_mode);
	std::string get_location();
	std::string get_line_text();
	void nested_file(char const *name);
	void nested_source(SrcText &source);
	SrcText &get_current();
};

#endif
