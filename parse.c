/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "parse.h"
#define NDEBUG
#include "debug.h"
#include <string.h>
#include "tkstream.h"
#include "error.h"
#include "opcodes.h"
#include "symtable.h"
#include "loc.h"
#include "emit.h"

/* use global variables because it's convenient */
static tkstream stream;
static token first;

/* first and second pass */

static void do_pass(void);
static int do_line(void);
static int do_opcode(void);
static int do_pseudo(void);
static int do_macrodef(void);
static int do_macro(void);
static void do_labeldef(void);
static int do_vardef(void);
static int do_location(void);
static u32 parse_expr(void);
static u16 expr_element(void);
static u16 parse_dec(char const *text);
static u16 parse_hex(char const *text);
static int expect_newline(void);

void first_pass(buffer_ref *in_buf) {
	DM(first pass)
	ts_init(&stream, in_buf);
	pass = 1;
	do_pass();
}

void second_pass(void) {
	DM(second pass)
	ts_rewind(&stream);
	pass = 2;
	do_pass();
}

void do_pass(void) {
	int ret;

	line = 1;
	while ((ret = do_line()) > 0) {
		++line;
	}

	if (ret < 0)
		error("Parse error");
}

int do_line(void) {
	int ret;

	ret = ts_read(&stream, &first);
	if (ret <= 0)
		return ret;

	switch (first.type) {
		case endline:
			return 1;
		case actual_opcode:
			return do_opcode();
		case pseudo_opcode:
			return do_pseudo();
		case macro_def:
			return do_macrodef();
		case macro_ref:
			return do_macro();
		case label_def:
			do_labeldef();
			return do_line();
		case var_def:
			return do_vardef();
		case '*':
			return do_location();
		default:
			return -1;
	}
}

int do_opcode(void) {
	token tk1;
	token tk2;
	u8 mode;
	u8 size;
	u32 value;

	ts_read(&stream, &tk1);

	switch (tk1.type) {
		case endline:
			mode = impl_mode; size = 1;
			ts_back(&stream);
			break;
		case '#':
			mode = imm_mode; size = 2;
			value = parse_expr();
			break;
		case '(':
			mode = ind_mode; size = 3;
			value = parse_expr();
			ts_read(&stream, &tk2);
			if (tk2.type == ',') {
				ts_read(&stream, &tk2);
				if (tk2.type == cpu_register)
					if (tk2.value[0] == 'X') {
						mode = indx_mode;
						size = 2;
					} else
						error("Invalid index register");
				else
					error("Index mode requires register");
				ts_read(&stream, &tk2);
			}
			if (tk2.type != ')') {
				error("Missing )");
			}
			ts_read(&stream, &tk2);
			if (tk2.type == ',') {
				ts_read(&stream, &tk2);
				if (tk2.type == cpu_register)
					if (tk2.value[0] == 'Y') {
						mode = indy_mode;
						size = 2;
					} else
						error("Invalid index register");
				else
					error("Index mode requires register");
			} else
				ts_back(&stream);
			break;
		case cpu_register:
			if (tk1.value[0] == 'A') {
				mode = impl_rega; size = 1;
				break;
			} else {
				error("Invalid register");
				return -1;
			}
		case symbol_ref:
		case literal_dec:
		case literal_hex:
		case literal_chr:
		case '!':
		case '*':
			mode = abs_mode; size = 3;
			ts_back(&stream);
			value = parse_expr();
			if (value <= 0x100) {
				mode = zp_mode; size = 2;
			}
			ts_read(&stream, &tk2);
			if (tk2.type != ',') {
				ts_back(&stream);
			} else {
				ts_read(&stream, &tk2);
				if (tk2.type == cpu_register) {
					switch (tk2.value[0]) {
						case 'X':
							if (mode == abs_mode)
								mode = absx_mode;
							else
								mode = zpx_mode;
							break;
						case 'Y':
							if (mode == abs_mode)
								mode = absy_mode;
							else
								mode = zpy_mode;
							break;
						default:
							error("Invalid index register");
							return -1;
					}
				} else {
					error("Index mode requires register");
					return -1;
				}
			}
			break;
		default:
			error("Syntax error");
	}

	emit_instruction(first.value[0], mode, (u16) value, size - 1);

	return expect_newline();
}

static void do_pseudo_word(void) {
	u16 value;
	token tk;

	do {
		value = (u16) parse_expr();
		emit_word(value);

		ts_read(&stream, &tk);

		if (tk.type == endline) {
			ts_back(&stream);
			break;
		}

		if (tk.type != ',')
			error("Syntax error");
	} while (1);
}
	
static void do_pseudo_byte(void) {
	u8 value;
	u16 len;
	token tk;

	do {
		ts_read(&stream, &tk);
		if (tk.type == literal_str) {
			len = strlen(tk.value);
			emit_bytes(tk.value, len);
		} else {
			ts_back(&stream);
			value = (u8) parse_expr();
			emit_byte(value);
		}

		ts_read(&stream, &tk);

		if (tk.type == endline) {
			ts_back(&stream);
			break;
		}

		if (tk.type != ',')
			error("Syntax error");
	} while (1);
}
	
static int do_pseudo(void) {
	switch (first.value[0]) {
		case 0: /* MACRO */
		case 1: /* ENDM */
			error("Macro not implemented");
			break;
		case 2: /* .BYTE */
			do_pseudo_byte();
			emit_next();
			break;
		case 3: /* .WORD */
			do_pseudo_word();
			emit_next();
			break;
		default:
			error("Internal error - no such pseudo-opcode");
	}

	return expect_newline();
}

static int expect_newline(void) {
	token tk;

	ts_read(&stream, &tk);
	if (tk.type != endline) {
		DV(tk.type,u8)
		error("Expected end of line");
		return -1;
	}

	return 1;
}

int do_macrodef(void) {
	error("Macros not implemented");
	return -1;
}

int do_macro(void) {
	error("Macros not implemented");
	return -1;
}

void make_local_label(char *local_label, char const *global_context, char const *local_part) {
	unsigned len1, len2;
	char *result;

	len1 = strlen(global_context);
	len2 = strlen(local_part);

	if (len1 + len2 > MAX_TOKEN_LENGTH)
		error("Label too long");

	memcpy(local_label + len1, local_part, len2 + 1);
	memcpy(local_label, global_context, len1);
}

static char main_label[MAX_TOKEN_LENGTH + 1];
void do_labeldef(void) {
	if (first.value[0] != '.') {  /* global label */
		strcpy(main_label, first.value);
	} else {                      /* local label */
		make_local_label(first.value, main_label, first.value);
	}

	if (pass == 1) {
		DV(first.value,str)
		sym_new(first.value, sym_label, location);
	}
}

int do_vardef(void) {
	token tk;
	u16 value;

	ts_read(&stream, &tk);
	if (tk.type != '=') {
		error("Expecting = for variable assignment");
		return -1;
	}

	value = (u16) parse_expr();

	if (first.value[0] == '.')
		make_local_label(first.value, main_label, first.value);

	sym_add(first.value, sym_var, value);

	return expect_newline();
}

int do_location(void) {
	token tk;

	ts_read(&stream, &tk);
	if (tk.type != '=') {
		error("Expecting = for location update");
		return -1;
	}

	location = (u16) parse_expr();

	return expect_newline();
}

u32 parse_expr(void) {
	token tk;
	u16 result;
	u16 operand;
	u8 operation;
	u32 long_result = 0;

	ts_read(&stream, &tk);
	if (tk.type == '!')
		long_result = 0x10000ul;
	else
		ts_back(&stream);

	result = expr_element();

	do {
		operation = 0;
		ts_read(&stream, &tk);
		switch (tk.type) {
			case '+':
			case '-':
				operation = tk.type;
				break;
			case ')':
			case endline:
			case ',':
				ts_back(&stream);
				break;
			default:
				error("Invalid expression");
				break;
		}

		if (operation != 0) {
			operand = expr_element();
			if (operation == '-')
				result -= operand;
			else
				result += operand;
		}
	} while (operation != 0);

	long_result |= result;

	return long_result;
}

u16 expr_element(void) {
	token tk;
	u16 value;

	ts_read(&stream, &tk);

	switch (tk.type) {
		case symbol_ref:
			if (tk.value[0] == '.')  /* local label */
				make_local_label(tk.value, main_label, tk.value);

			if (sym_get(tk.value, sym_any, &value) != 0)
				return value;
			else
				if (pass == 1)
					return location;
				else
					error_str("Symbol %s not found", tk.value);
		case literal_chr:
			return (u16)tk.value[0];
		case literal_dec:
			return parse_dec(tk.value);
		case literal_hex:
			return parse_hex(tk.value);
		case '<':
			return expr_element() & 0xFF;
		case '>':
			return (expr_element()) >> 8 & 0xFF;
		default:
			error("Invalid expression");
			return (u16) -1;
	}
}

u16 parse_dec(char const *text) {
	u16 value = 0;
	u8 c;
	char const *p;

	c = *text;
	if (c == 0 || c < '0' || c > '9') goto error;
	value = (u16) (c - '0');

	for (p = text + 1; (c = *p) != 0; ++p) {
		if (c < '0' || c > '9') goto error;
		value *= 10;
		value += (u16)(c - '0');
	}

	return value;

error:
	error_str("Invalid decimal number %s", text);
	return (u16) -1;
}

u16 parse_hex(char const *text) {
	u16 value = 0;
	char const *p;
	u8 c;
	u8 x;

	c = *text;
	if (c == 0 || c < '0' || c > 'F' || (c > '9' && c < 'A'))
		goto error;
	x = c - '0';
		if (c >= 'A') x -= ('A' - '9' - 1);
	value = x;

	for (p = text + 1; (c = *p) != 0; ++p) {
		if (c == 0 || c < '0' || c > 'F' || (c > '9' && c < 'A'))
			goto error;
		value <<= 4;
		x = c - '0';
		if (c >= 'A')
			x -= ('A' - '9' - 1);
		value += x;
	}

	return value;

error:
	error("Invalid decimal number");
	return (u16) -1;
}

