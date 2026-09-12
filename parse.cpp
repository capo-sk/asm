/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "parse.hpp"
#include <cstring>
#include <cstdarg>
#include "opcodes.hpp"
#include "emit.hpp"
#include "error.h"

Parser::Parser(Buffer &in_buf, Emitter &emitter, SymbolTable &symtab)
	: stream(in_buf), emit(emitter), sym(symtab) {
	pass = 1;
}
	
void Parser::first_pass() {
	pass = 1;
	emit.set_pass(1);
	do_pass();
}

void Parser::second_pass() {
	stream.reset();
	pass = 2;
	emit.set_pass(2);
	do_pass();
}

void Parser::do_pass() {
	int ret;

	while ((ret = do_line()) > 0)
		stream.AdvanceLine();

	if (ret < 0)
		error("Parse error");
}

int Parser::do_line() {
	int ret;

	ret = stream.read(first);
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

int Parser::do_opcode() {
	Token tk1;
	Token tk2;
	u8 mode;
	u8 size;
	u32 value;

	stream.read(tk1);

	switch (tk1.type) {
		case endline:
			mode = impl_mode; size = 1;
			stream.rewind_1();
			break;
		case '#':
			mode = imm_mode; size = 2;
			value = parse_expr();
			break;
		case '(':
			mode = ind_mode; size = 3;
			value = parse_expr();
			stream.read(tk2);
			if (tk2.type == ',') {
				stream.read(tk2);
				if (tk2.type == cpu_register)
					if (tk2.value[0] == 'X') {
						mode = indx_mode;
						size = 2;
					} else
						error("Invalid index register");
				else
					error("Index mode requires register");
				stream.read(tk2);
			}
			if (tk2.type != ')') {
				error("Missing )");
			}
			stream.read(tk2);
			if (tk2.type == ',') {
				stream.read(tk2);
				if (tk2.type == cpu_register)
					if (tk2.value[0] == 'Y') {
						mode = indy_mode;
						size = 2;
					} else
						error("Invalid index register");
				else
					error("Index mode requires register");
			} else
				stream.rewind_1();
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
			stream.rewind_1();
			value = parse_expr();
			if (value <= 0x100) {
				mode = zp_mode; size = 2;
			}
			stream.read(tk2);
			if (tk2.type != ',') {
				stream.rewind_1();
			} else {
				stream.read(tk2);
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

	emit.emit_instruction(first.value[0], mode, (u16) value, size - 1);

	return expect_newline();
}

void Parser::do_pseudo_word() {
	u16 value;
	Token tk;

	do {
		value = (u16) parse_expr();
		emit.emit_word(value);

		stream.read(tk);

		if (tk.type == endline) {
			stream.rewind_1();
			break;
		}

		if (tk.type != ',')
			error("Syntax error");
	} while (1);
}
	
void Parser::do_pseudo_byte() {
	u8 value;
	u16 len;
	Token tk;

	do {
		stream.read(tk);
		if (tk.type == literal_str) {
			len = strlen(tk.value);
			emit.emit_bytes((u8 *)tk.value, len);
		} else {
			stream.rewind_1();
			value = (u8) parse_expr();
			emit.emit_byte(value);
		}

		stream.read(tk);

		if (tk.type == endline) {
			stream.rewind_1();
			break;
		}

		if (tk.type != ',')
			error("Syntax error");
	} while (1);
}
	
int Parser::do_pseudo(void) {
	switch (first.value[0]) {
		case 0: /* MACRO */
			do_macro_header();
			break;
		case 1: /* ENDM */
			error("Macro not implemented");
			break;
		case 2: /* .BYTE */
			do_pseudo_byte();
			break;
		case 3: /* .WORD */
			do_pseudo_word();
			break;
		case 4: /* .INCLUDE */
			do_include();
			break;
		default:
			error("Internal error - no such pseudo-opcode");
	}

	return expect_newline();
}

void Parser::do_macro_header() {
	Token tk;

	stream.read(tk);
	if (tk.type != macro_def)
		error("Expected macro name");

	if (pass == 1)
		sym.addnew(tk.value, sym_macro, 0);

	Token tk2;
	u16 ct;
	char *cp;
	stream.read(tk2);
	ct = 1;
	while (tk2.type == macro_par) {
		if (pass == 1) {
			char parsym[VALUE_SIZE];

			cp = stpncpy(parsym, tk.value, VALUE_SIZE - 2);
			*cp++ = '&';
			strncpy(cp, tk2.value, VALUE_SIZE - strlen(tk.value) - 2);

			sym.addnew(parsym, sym_param, ct);

			ct++;
		}
		stream.read(tk2);
	}
	stream.rewind_1();
}

int Parser::do_include() {
	Token tk;
	int result;

	stream.read(tk);
	if (tk.type != literal_str)
		error("Expected filename");

	result = expect_newline();
	stream.rewind_1();

	stream.nested_file(tk.value);

	return result;
}

int Parser::expect_newline() {
	Token tk;

	stream.read(tk);
	if (tk.type != endline) {
		error("Expected end of line");
		return -1;
	}

	return 1;
}

int Parser::do_macrodef(void) {
	error("Macros not implemented");
	return -1;
}

int Parser::do_macro(void) {
	error("Macros not implemented");
	return -1;
}

void Parser::make_local_label(char *local_label, char const *global_context, char const *local_part) {
	unsigned len1, len2;
	char *result;

	len1 = strlen(global_context);
	len2 = strlen(local_part);

	if (len1 + len2 > MAX_TOKEN_LENGTH)
		error("Label too long");

	memmove(local_label + len1, local_part, len2 + 1);
	memmove(local_label, global_context, len1);
}

void Parser::do_labeldef(void) {
	if (first.value[0] != '.') {  /* global label */
		main_label = first;
	} else {                      /* local label */
		make_local_label(first.value, main_label.value, first.value);
	}

	if (pass == 1) {
		sym.addnew(first.value, sym_label, emit.get_loc());
	}
}

int Parser::do_vardef(void) {
	Token tk;
	u16 value;

	stream.read(tk);
	if (tk.type != '=') {
		error("Expecting = for variable assignment");
		return -1;
	}

	value = (u16) parse_expr();

	if (first.value[0] == '.')
		make_local_label(first.value, main_label.value, first.value);

	sym.add(first.value, sym_var, value);

	return expect_newline();
}

int Parser::do_location(void) {
	Token tk;

	stream.read(tk);
	if (tk.type != '=') {
		error("Expecting = for location update");
		return -1;
	}

	emit.set_loc((u16) parse_expr());

	return expect_newline();
}

u32 Parser::parse_expr(void) {
	Token tk;
	u16 result;
	u16 operand;
	u8 operation;
	u32 long_result = 0;

	stream.read(tk);
	if (tk.type == '!')
		long_result = 0x10000ul;
	else
		stream.rewind_1();

	result = expr_element();

	do {
		operation = 0;
		stream.read(tk);
		switch (tk.type) {
			case '+':
			case '-':
				operation = tk.type;
				break;
			case ')':
			case endline:
			case ',':
				stream.rewind_1();
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

u16 Parser::expr_element(void) {
	Token tk;
	u16 value;

	stream.read(tk);

	switch (tk.type) {
		case symbol_ref:
			if (tk.value[0] == '.')  /* local label */
				make_local_label(tk.value, main_label.value, tk.value);

			if (sym.get(tk.value, sym_anynum, value) != 0)
				return value;
			else
				if (pass == 1)
					return emit.get_loc();
				else
					error_fmt("Symbol %s not found", tk.value);
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

u16 Parser::parse_dec(char const *text) {
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
	error_fmt("Invalid decimal number %s", text);
}

u16 Parser::parse_hex(char const *text) {
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
}

[[noreturn]] void Parser::error(char const *txt) {
	abort_fmt("%s(%u): %s", stream.getFilename(), stream.getLinenum(), txt);
}

[[noreturn]] void Parser::error_fmt(char const *fmt, ...) {
	va_list args;
	char msg[256];

	va_start(args, fmt);
	vsprintf(msg, fmt, args);
	error(msg);
	va_end(args);
}
