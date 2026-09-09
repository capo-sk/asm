/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "tkstream.hpp"
#include <cctype>
#include <cstring>
#include "opcodes.hpp"

TokenStream::TokenStream(Buffer &in_buf) : buf(in_buf) {
	ctx = line_start;
	reuse = false;
}

int TokenStream::read(Token &tk) {
	int retval;

	if (reuse) {
		reuse = false;
		retval = 1;
	} else {
		retval = _get_next_token(&latest);
	}

	tk = latest;

	return retval;
}

void TokenStream::rewind_1() {
	reuse = true;
}

void TokenStream::rewind() {
	buf.rewind();
	reuse = false;
	ctx = line_start;
}

void TokenStream::reset() {
	buf.reset();
	rewind();
}


/*
return: tk is populated with the next token
return: 1 if valid token, 0 if eof, -1 error
update: in_buf as consumed
update: ctx with new context if changed
*/
int TokenStream::_get_next_token(Token *tk) {
	int c;
	unsigned char literal_delimiter;
	unsigned i;
	bool incomplete_token = true;
	unsigned text_len = 0;
	bool inside_string_literal = false;
	bool inside_comment = false;

	tk->type = invalid;  /* until we know better */

	while (incomplete_token) {
		c = buf.get_next();

		if (c == Buffer::eofmark) {
			if (text_len == 0) {  // reached end of file
				tk->type = endline;
				tk->value[0] = 0;
				if (buf.close_file() == 0) { // no more files
					ctx = end_of_file;
					return 0;
				} else {
					ctx = line_start;
					return 1;
				}
			} else {  // EOF terminates word
				buf.rewind_1();
				tk->value[text_len] = 0;
				incomplete_token = false;
				break;
			}
		}

		/* if we are inside a string literal, accumulate until we
		   find the end delimiter */
		if (inside_string_literal) {
			if (c == literal_delimiter) {
				tk->value[text_len] = 0;
				if (literal_delimiter == '"')
					tk->type = literal_str;
				else
					tk->type = literal_chr;
				goto update_ctx;
			}
			if (text_len < MAX_TOKEN_LENGTH) {
				tk->value[text_len] = c;
				++text_len;
				continue;
			} else {
				ctx = err_too_long;
				return -1;
			}
		}

		/* if we are in a comment, ignore all until newline */
		if (inside_comment) {
			if (c == 13 || c == 10) {  // newline ends comment
				tk->type = endline;
				tk->value[0] = 0;
				ctx = line_start;
				return 1;
			}
			continue;
		}

		/* convert to uppercase */
		if (islower(c))
			c = toupper(c);

		/* start char/string literal 'c' "str" */
		if (
		    c == '"' ||
		    c == '\''
		   ) {
			if (text_len == 0) {
				literal_delimiter = c;
				inside_string_literal = true;
				continue;
			} else {
				tk->value[text_len] = 0;
				buf.rewind_1();
				incomplete_token = false;
				break;
			}
		}

		/* start comment */
		if (c == ';') {
			if (text_len == 0) {
				inside_comment = true;
				continue;
			} else {
				tk->value[text_len] = 0;
				buf.rewind_1();
				incomplete_token = false;
				break;
			}
		}
			
		/* special symbols */
		if (
		    c == '#' ||
		    c == '*' ||
		    c == '=' ||
		    c == '!' ||
		    c == '<' ||
		    c == '>' ||
		    c == ',' ||
		    c == '+' ||
		    c == '-' ||
		    c == '(' ||
		    c == ')' ||
		    c == ',' ||
		    c == 13 ||	/* cr */
		    c == 10	/* lf */
		   ) {
			if (text_len == 0) {	/* c is the token */
				if (c == 13 || c == 10) {
					tk->type = endline;
					ctx = line_start;
				} else {
					tk->type = (token_type) c;
					ctx = line_middle;
				}
				tk->value[0] = 0;  /* no value */
				return 1;
			} else {		/* c is not part of the token */
				buf.rewind_1();
				tk->value[text_len] = 0;
				incomplete_token = false;
				break;
			}
		}

		/* whitespace */
		if (
		    c == ' ' ||
		    c == 9  /* tab */
		   ) {
			if (text_len == 0) {
				/* ignore starting whitespace */
				continue;
			} else {
				/* end of word */
				tk->value[text_len] = 0;
				incomplete_token = false;
				break;
			}
		}

		/* colon signals end of label */
		if (c == ':') {
			if (text_len == 0) {
				/* bare colon */
				tk->type = invalid;
				tk->value[0] = c;
				tk->value[1] = 0;
				ctx = line_middle;
				return 1;
			} else {
				tk->type = label_def;
				tk->value[text_len] = 0;
				ctx = line_middle;
				return 1;
			}
		}
			
		/* word */
		if (
		    (c >= 'A' && c <= 'Z') ||
		    (c >= '0' && c <= '9') ||
		    c == '_' ||
		    c == '$' ||
		    c == '.' ||
		    c == '&'
		   ) {
			if (text_len <= MAX_TOKEN_LENGTH) {
				tk->value[text_len] = c;
				++text_len;
				continue;
			} else {
				ctx = err_too_long;
				return -1;
			}
		}

		/* invalid character */
		tk->value[0] = c;
		tk->value[1] = 0;
		ctx = err_invalid_chr;
		return -1;
	}

	if (tk->type == invalid) {
		c = tk->value[0];
		if (c == '$') {
			tk->type = literal_hex;
			memmove(tk->value, tk->value + 1, text_len);
		} else if (c >= '0' && c <= '9')
			tk->type = literal_dec;
		else if (c == '&') {
			tk->type = macro_ref;
			memmove(tk->value, tk->value + 1, text_len);
		} else {
			/* is it a register? */
			for (i = 0; i < num_registers; ++i) {
				if (strcmp(tk->value, cpu_registers[i]) == 0) {
					tk->type = cpu_register;
					goto update_ctx;
				}
			}

			/* is it an opcode? */
			for (i = 0; i < num_opcodes; ++i) {
				if (strcmp(tk->value, opcodes[i].mnemonic) == 0) {
					tk->value[0] = (u8)i;
					tk->type = actual_opcode;
					goto update_ctx;
				}
			}

			/* is it a pseudo-opcode? */
			for (i = 0; i < num_pseudos; ++i) {
				if (strcmp(tk->value, pseudos[i].mnemonic) == 0) {
					tk->value[0] = (u8)i;
					tk->type = pseudo_opcode;
					if (i == pseudo_macro)
						ctx = macro_header1;
					goto update_ctx;
				}
			}

			/* what type of identifier is it? */
			if (ctx == line_start) {
				tk->type = var_def;
			} else if (ctx == macro_header1) {
				tk->type = macro_def;
				ctx = macro_header2;
			} else if (ctx == macro_header2) {
				tk->type = macro_par;
			} else {
				tk->type = symbol_ref;
			}
		}
	}

update_ctx:
	if (ctx == line_start)
		ctx = line_middle;

	return 1;
}

void TokenStream::AdvanceLine() {
	buf.AdvanceLine();
}

char const *TokenStream::getFilename() {
	return buf.getFilename();
}

unsigned TokenStream::getLinenum() {
	return buf.getLinenum();
}

void TokenStream::nested_file(char const *name) {
	buf.new_file(name);
}

