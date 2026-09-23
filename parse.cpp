/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "parse.hpp"
#include <cstring>
#include <cstdarg>
#include "opcodes.hpp"
#include "emit.hpp"
#include "macro.hpp"
#include "error.h"

using namespace std;

Parser::Parser(Buffer &in_buf, Emitter &emitter, SymbolTable &symtab)
	: stream(in_buf), emit(emitter), sym(symtab) {
	pass = 1;
	uniq = 0;
}
	
void Parser::first_pass() {
	main_label.value[0] = 0;
	pass = 1;
	emit.set_pass(pass);
	uniq = 0;
	p0_source();
}

void Parser::second_pass() {
	stream.reset();
	main_label.value[0] = 0;
	pass = 2;
	emit.set_pass(pass);
	uniq = 0;
	p0_source();
}

void Parser::p0_source() {
	int ret;

	while ((ret = p1_line()) > 0)
		;
		//stream.AdvanceLine();

	if (ret < 0)
		error("Parse error");
}

int Parser::p1_line() {
	int ret;

	ret = stream.read(first);
	if (ret <= 0)
		return ret;

	switch (first.type) {
		case endline:
			return 1;
		case actual_opcode:
			return p2_instruction();
		case pseudo_opcode:
			return p2_pseudo_instruction();
		case macro_ref:
			return p2_invoke_macro();
		case label_def:
			p2_labeldef();
			return p1_line();
		case var_def:
			return p2_vardef();
		case '*':
			return p2_location();
		case macro_line:
			return p2_macro_line();
		case skip:
			return p1_line();
		default:
			return -1;
	}
}

int Parser::p2_instruction() {
	Token tk1;
	Token tk2;
	uint8_t mode;
	uint8_t size;
	uint32_t value;

	stream.read(tk1);

	switch (tk1.type) {
		case endline:
			mode = impl_mode; size = 1;
			stream.pushback();
			break;
		case '#':
			mode = imm_mode; size = 2;
			value = p3_expression();
			break;
		case '(':
			mode = ind_mode; size = 3;
			value = p3_expression();
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
				stream.pushback();
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
		case '^':
			mode = abs_mode; size = 3;
			stream.pushback();
			value = p3_expression();
			if (value <= 0x100) {
				mode = zp_mode; size = 2;
			}
			stream.read(tk2);
			if (tk2.type != ',') {
				stream.pushback();
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

	emit.emit_instruction(first.value[0], mode, (uint16_t) value, size - 1);

	return expect_newline();
}

void Parser::p3_pseudo_word() {
	uint16_t value;
	Token tk;

	do {
		value = (uint16_t) p3_expression();
		emit.emit_word(value);

		stream.read(tk);

		if (tk.type == endline) {
			stream.pushback();
			break;
		}

		if (tk.type != ',')
			error("Syntax error");
	} while (1);
}
	
void Parser::p3_pseudo_byte()
{
	uint8_t value;
	uint16_t len;
	Token tk;

	do {
		stream.read(tk);
		if (tk.type == literal_str) {
			len = strlen(tk.value);
			emit.emit_bytes((uint8_t *)tk.value, len);
		} else {
			stream.pushback();
			value = (uint8_t) p3_expression();
			emit.emit_byte(value);
		}

		stream.read(tk);

		if (tk.type == endline) {
			stream.pushback();
			break;
		}

		if (tk.type != ',')
			error("Syntax error");
	} while (1);
}
	
void Parser::p3_pseudo_align()
{
	uint16_t value, inc, mask;
	uint16_t target_loc;
	Token tk;
	uint8_t zero = 0;

	value = p3_expression();
	inc = value - 1;
	mask = ~inc;
	target_loc = (emit.get_loc() + inc) & mask;
	
	while (emit.get_loc() != target_loc)
		emit.emit_byte(zero);
}

int Parser::p2_pseudo_instruction()
{
	switch (first.value[0]) {
		case 0: /* MACRO */
			p3_macro_header();
			return 1;  // already consumed newline
		case 1: /* ENDM */
			stream.set_mode(assembly);
			return 1;
		case 2: /* .BYTE */
			p3_pseudo_byte();
			break;
		case 3: /* .WORD */
			p3_pseudo_word();
			break;
		case 4: /* .INCLUDE */
			return p3_include();
		case 5: /* .ALIGN */
			p3_pseudo_align();
			break;
		default:
			error("Internal error - no such pseudo-opcode");
	}

	return expect_newline();
}

void Parser::p3_macro_header() {
	Token tk;

	stream.read(tk);
	if (tk.type != macro_def)
		error("Expected macro name");

	if (pass == 1) {
		current_macro = new MacroText(tk.value);
	}

	unsigned parct = 0;
	Token tk2;
	stream.read(tk2);
	while (tk2.type == macro_par) {
		if (pass == 1) {
			current_macro->add_param(tk2.value);
			parct++;
		}
		stream.read(tk2);
	}

	if (pass == 1) {
		sym.add_unique(tk.value, sym_macro, parct);
		macros.add(*current_macro);
	}

	pushback_and_newline();

	// switch to macro mode
	stream.set_mode(macro);
}

int Parser::p2_macro_line()
{
	if (pass == 1)
		current_macro->add_line(first.value);
	return 1;
}

int Parser::p3_include() {
	Token tk;
	int result;

	stream.read(tk);
	if (tk.type != literal_str)
		error("Expected filename");

	result = expect_newline();
	//stream.pushback();

	stream.nested_file(tk.value);

	return result;
}

int Parser::expect_newline() {
	Token tk;

	do {
		stream.read(tk);
	} while (tk.type == skip);

	if (tk.type != endline) {
		error("Expected end of line");
		return -1;
	}

	return 1;
}

int Parser::pushback_and_newline() {
	stream.pushback();
	return expect_newline();
}

int Parser::p2_invoke_macro(void) {
	// increase uniq counter
	++uniq;

	// find macro by name
	auto macro_it = macros.find(first.value);
	if (macro_it == macros.end())
		error_fmt("Unknown macro %s", first.value);

	// parse parameters
	auto *values = new list<string>;
	Token tk;
	stream.set_mode(words);
	stream.read(tk);
	while (tk.type == word) {
		values->push_back(tk.value);
		stream.read(tk);
	}
	stream.set_mode(assembly);
	pushback_and_newline();

	// invoke macro with parameters
	macro_it->second.invoke(stream.get_current(), *values);

	// switch input to macro
	stream.nested_source(macro_it->second);

	return 1;
}

void Parser::localise_label(Token &tk)
{
	string result;

	if (tk.value[0] == '.') {
		if (tk.value[1] == '.') {  // macro unique label
			result = string(stream.get_current().get_name() + to_string(uniq) + string(&tk.value[1]));
		} else {  // local label
			result = string(main_label.value) + string(tk.value);
		}

		if (result.length() > MAX_TOKEN_LENGTH)
			error_fmt("Symbol %s too long", result.c_str());
	
		strcpy(tk.value, result.c_str());
	}
}

/*
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
*/

void Parser::p2_labeldef(void) {
	if (first.value[0] != '.') {  /* global label */
		main_label = first;
	} else {                      /* local label */
		localise_label(first);
		//make_local_label(first.value, main_label.value, first.value);
	}

	if (pass == 1) {
		sym.add_unique(first.value, sym_label, emit.get_loc());
	}
}

int Parser::p2_vardef(void) {
	Token tk;
	uint16_t value;

	stream.read(tk);
	if (tk.type != '=') {
		error("Expecting = for variable assignment");
		return -1;
	}

	value = (uint16_t) p3_expression();

	if (first.value[0] == '.')
		localise_label(first);
		//make_local_label(first.value, main_label.value, first.value);

	sym.add_or_overwrite(first.value, sym_var, value);

	return expect_newline();
}

int Parser::p2_location(void) {
	Token tk;

	stream.read(tk);
	if (tk.type != '=') {
		error("Expecting = for location update");
		return -1;
	}

	emit.set_loc((uint16_t) p3_expression());

	return expect_newline();
}

uint32_t Parser::p3_expression(void) {
	Token tk;
	uint16_t result;
	uint16_t operand;
	uint8_t operation;
	uint32_t long_result = 0;

	stream.read(tk);
	if (tk.type == '!')
		long_result = 0x10000ul;
	else
		stream.pushback();

	result = p4_expr_element();

	do {
		operation = 0;
		stream.read(tk);
		switch (tk.type) {
			case '+':
			case '-':
			case '*':
			case '/':
				operation = tk.type;
				break;
			case ')':
			case endline:
			case ',':
				stream.pushback();
				break;
			default:
				error("Invalid expression");
				break;
		}

		if (operation != 0) {
			operand = p4_expr_element();
			if (operation == '-')
				result -= operand;
			else if (operation == '+')
				result += operand;
			else if (operation == '*')
				result *= operand;
			else if (operation == '/')
				result /= operand;
			else
			 	;  // can't be here
		}
	} while (operation != 0);

	long_result |= result;

	return long_result;
}

uint16_t Parser::p4_expr_element(void) {
	Token tk;
	uint16_t value;

	stream.read(tk);

	switch (tk.type) {
		case symbol_ref:
			if (tk.value[0] == '.')  /* local label */
				localise_label(tk);
				//make_local_label(tk.value, main_label.value, tk.value);

			if (sym.get(tk.value, sym_anynum, value) != 0)
				return value;
			else
				if (pass == 1)
					return emit.get_loc();
				else
					error_fmt("Symbol %s not found", tk.value);
		case literal_chr:
			return (uint16_t)tk.value[0];
		case literal_dec:
			return p5_dec(tk.value);
		case literal_hex:
			return p5_hex(tk.value);
		case literal_bin:
			return p5_bin(tk.value);
		case '<':
			return p4_expr_element() & 0xFF;
		case '>':
			return (p4_expr_element()) >> 8 & 0xFF;
		case '^':
			return p4_expr_element() << 8;
		case '*':
			return emit.get_loc();
		default:
			error("Invalid expression");
			return (uint16_t) -1;
	}
}

uint16_t Parser::p5_dec(char const *text) {
	uint16_t value = 0;
	uint8_t c;
	char const *p;

	c = *text;
	if (c == 0 || c < '0' || c > '9') goto error;
	value = (uint16_t) (c - '0');

	for (p = text + 1; (c = *p) != 0; ++p) {
		if (c < '0' || c > '9') goto error;
		value *= 10;
		value += (uint16_t)(c - '0');
	}

	return value;

error:
	error_fmt("Invalid decimal number %s", text);
}

uint16_t Parser::p5_bin(char const *text)
{
	uint16_t value = 0;
	char const *p;
	char c;

	for (p = text; *p != 0; ++p) {
		value <<= 1;
		switch (*p) {
			case '0': 
				break;
			case '1':
				value |= 1;
				break;
			default:
				error_fmt("Invalid binary literal %s", text);
		}
	}

	return value;
}

uint16_t Parser::p5_hex(char const *text)
{
	uint16_t value = 0;
	char const *p;
	uint8_t c;
	uint8_t x;

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
	abort_fmt("%s: %s\n%s\n", stream.get_location().c_str(), txt, stream.get_line_text().c_str());
}

[[noreturn]] void Parser::error_fmt(char const *fmt, ...) {
	va_list args;
	char msg[256];

	va_start(args, fmt);
	vsprintf(msg, fmt, args);
	error(msg);
	va_end(args);
}
