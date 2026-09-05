/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* TOKENS */

#include "token.h"
#define NDEBUG
#include "debug.h"
#include <string.h>
#include <ctype.h>
#include "opcodes.h"


/*
return: tk is populated with the next token
return: 1 if valid token, 0 if eof, -1 error
update: in_buf as consumed
update: ctx with new context if changed
*/
int get_next_token(buffer_ref *in_buf, token_context *ctx, token *tk) {
	char c;
	char literal_delimiter;
	u8 i;
	bool incomplete_token = true;
	u8 text_len = 0;
	bool inside_string_literal = false;
	bool inside_comment = false;

	tk->type = invalid;  /* until we know better */

	while (incomplete_token) {
		c = buf_get_next_char(in_buf);
		debug_var(c, i8);

		if (c == (char) buf_eof) {
			if (text_len == 0) {
				debug_msg(end of file);
				tk->type = endline;
				tk->value[0] = 0;
				*ctx = end_of_file;
				return 0;
			} else {
				debug_msg(eof terminates word);
				buf_rewind_1(in_buf);
				tk->value[text_len] = 0;
				incomplete_token = false;
				break;
			}
		}

		/* if we are inside a string literal, accumulate until we
		   find the end delimiter */
		if (inside_string_literal) {
			debug_msg(literal);
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
				debug_msg(token too long);
				*ctx = err_too_long;
				return -1;
			}
		}

		/* if we are in a comment, ignore all until newline */
		if (inside_comment) {
			debug_msg(comment);
			if (c == 13 || c == 10) {
				debug_msg(newline ends comment);
				tk->type = endline;
				tk->value[0] = 0;
				*ctx = line_start;
				return 1;
			}
			continue;
		}

		/* convert to uppercase */
		if (islower(c))
			c = toupper(c);
		debug_var(c, i8);

		/* start char/string literal 'c' "str" */
		if (
		    c == '"' ||
		    c == '\''
		   ) {
			if (text_len == 0) {
				debug_msg(quotes);
				literal_delimiter = c;
				inside_string_literal = true;
				continue;
			} else {
				debug_msg(quotes terminate word);
				tk->value[text_len] = 0;
				buf_rewind_1(in_buf);
				incomplete_token = false;
				break;
			}
		}

		/* start comment */
		if (c == ';') {
			if (text_len == 0) {
				debug_msg(comment);
				inside_comment = true;
				continue;
			} else {
				debug_msg(comment terminates word);
				tk->value[text_len] = 0;
				buf_rewind_1(in_buf);
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
					debug_msg(newline);
					tk->type = endline;
					*ctx = line_start;
				} else {
					debug_msg(standalone);
					tk->type = (token_type) c;
					*ctx = line_middle;
				}
				tk->value[0] = 0;  /* no value */
				return 1;
			} else {		/* c is not part of the token */
				debug_msg(special terminates word);
				buf_rewind_1(in_buf);
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
				debug_msg(ignoring whitespace);
				continue;
			} else {
				/* end of word */
				debug_msg(space terminates word);
				tk->value[text_len] = 0;
				incomplete_token = false;
				break;
			}
		}

		/* colon signals end of label */
		if (c == ':') {
			if (text_len == 0) {
				/* bare colon */
				debug_msg(bare colon);
				tk->type = invalid;
				tk->value[0] = c;
				tk->value[0] = 0;
				*ctx = line_middle;
				return 1;
			} else {
				debug_msg(colon ends label);
				tk->type = label_def;
				tk->value[text_len] = 0;
				*ctx = line_middle;
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
			debug_msg(word character);
			if (text_len < MAX_TOKEN_LENGTH) {
				tk->value[text_len] = c;
				++text_len;
				continue;
			} else {
				debug_msg(token too long);
				*ctx = err_too_long;
				return -1;
			}
		}

		/* invalid character */
		debug_msg(invalid character);
		tk->value[0] = c;
		tk->value[1] = 0;
		*ctx = err_invalid_chr;
		return -1;
	}

	if (tk->type == invalid) {
		c = tk->value[0];
		if (c == '$') {
			tk->type = literal_hex;
			memcpy(tk->value, tk->value + 1, text_len);
		} else if (c >= '0' && c <= '9')
			tk->type = literal_dec;
		else if (c == '&') {
			tk->type = macro_ref;
			memcpy(tk->value, tk->value + 1, text_len);
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
						*ctx = macro_header1;
					goto update_ctx;
				}
			}

			/* what type of identifier is it? */
			if (*ctx == line_start) {
				debug_msg(first on line is a variable def);
				tk->type = var_def;
			} else if (*ctx == macro_header1) {
				debug_msg(macro name);
				tk->type = macro_def;
				*ctx = macro_header2;
			} else if (*ctx == macro_header2) {
				debug_msg(macro parameter);
				tk->type = macro_par;
			} else {
				debug_msg(middle of line is a symbol reference);
				tk->type = symbol_ref;
			}
		}
	}

update_ctx:
	if (*ctx == line_start)
		*ctx = line_middle;

	return 1;
}
