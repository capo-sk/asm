/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "parse.hpp"
#include "opcodes.hpp"
#include "emit.hpp"
#include "macro.hpp"

//#include <cstddef>
#include <cstdint>
#include <string>
#include <format>

using std::string;

Parser::Parser(Buffer &in_buf, Emitter &emitter, SymbolTable &symtab)
	: stream(in_buf), emit(emitter), sym(symtab) {
	pass = 1;
	uniq = 0;
}
	
void Parser::first_pass() {
	pass = 1;
	p0_source();
}

void Parser::second_pass() {
	stream.reset();
	pass = 2;
	p0_source();
}

void Parser::p0_source() {
	emit.set_pass(pass);
	emit.set_loc(0);
	main_label.clear();
	uniq = 0;

	int ret;
	while ((ret = p1_line()) > 0)
		;

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
					if (tk2.number() == 'X') {
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
					if (tk2.number() == 'Y') {
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
			if (tk1.number() == 'A') {
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
					switch (tk2.number()) {
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

	emit.emit_instruction(first.number(), mode, (uint16_t) value, size - 1);

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
			len = tk.value.length();  // ignoring the case that length > 64K
			emit.emit_bytes((uint8_t *)tk.value.c_str(), len);
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
	const uint8_t zero = 0;

	uint16_t value = p3_expression();
	if (value == 0 || value >0x8000)
		error(std::format("Invalid align multiple {}", value));

	uint16_t odd = emit.get_loc() % value;
	if (odd > 0)
		for (uint16_t count = value - odd; count; --count)
			emit.emit_byte(zero);
}

int Parser::p2_pseudo_instruction()
{
	switch (first.number()) {
		case 0: /* MACRO */
			p3_macro_header();
			return 1;  // already consumed newline
		case 1: /* ENDM */
			stream.set_mode(assembly);
			current_macro = NULL;
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

void Parser::p3_macro_header()
{
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
		error(format("Unknown macro {}", first.value));

	// prevent recursive macro
	if (macro_it->first == stream.get_current().get_name())
		error("Macro may not invoke itself");

	// parse parameters
//	auto *values = new list<string>;
	MacroText &macro = macro_it->second;
	macro.new_invocation();
	Token tk;
	stream.set_mode(words);
	stream.read(tk);
	while (tk.type == word) {
		macro.add_value(tk.value);
//		values->push_back(tk.value);
		stream.read(tk);
	}
	stream.set_mode(assembly);
	pushback_and_newline();

	// number of values must match number of parameters
	if (macro.get_value_count() != macro.get_param_count())
		error("Argument count mismatch");

	// invoke macro with parameters
//	macro_it->second.invoke(stream.get_current(), *values);

	// switch input to macro
	stream.nested_source(macro_it->second);

	return 1;
}

void Parser::localise_symbol(Token &tk)
{
	string result;

	if (tk.value.length() > 1 && tk.value[0] == '.') {
		if (typeid(stream.get_current()) == typeid(MacroText) && tk.value.length() > 2 && tk.value[1] == '.') {
			// macro unique label
			result = string(stream.get_current().get_name() + std::to_string(uniq) + string(&tk.value[1]));
		} else {
			// local label
			result = string(main_label.value) + string(tk.value);
		}

//		if (result.length() > MAX_TOKEN_LENGTH)
//			error(format("Symbol {} too long", result));
	
		tk.value = result;
	}
}

void Parser::p2_labeldef(void) {
	// first is the label token; we know length is >= 1
	if (first.value[0] != '.') {  /* global label */
		main_label = first;
	} else {                      /* local label */
		localise_symbol(first);
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

	// local variable
	if (first.value[0] == '.')
		localise_symbol(first);

	sym.add_or_overwrite(first.value, sym_var, value);

	return expect_newline();
}

int Parser::p2_location(void)
{
	Token tk;
	uint16_t value;

	stream.read(tk);
	if (tk.type != '=') {
		error("Expecting = for location update");
		return -1;
	}

	value = (uint16_t) p3_expression();
	emit.set_loc(value);

	return expect_newline();
}

uint32_t Parser::p3_expression()
{
	Token tk;
	uint64_t result;
	uint64_t operand;
	uint32_t force_word = 0;
	int operation;

	stream.read(tk);
	if (tk.type == '!')
		force_word = 0x10000ul;
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
//			case ')':
//			case endline:
//			case ',':
//				stream.pushback();
//				break;
			default:
				stream.pushback();
//				error("Invalid expression");
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
			else if (operation == '/') {
				if (operand == 0)
					error("Division by zero");
				result /= operand;
			} else
			 	;  // can't be here
		}
	} while (operation != 0);

	int64_t max_uword = 65535L;
	int64_t min_sword = -32768;

	// calculations use unsigned arithmetics but for this final check
	// we accept both unsigned/positive values up to 65535
	// and 2-complement values down to -32768
	int64_t sresult = (int64_t) result;
	if (sresult > max_uword || sresult < min_sword)
		error(std::format("Expression value is larger than 16 bits ({})", sresult));

	return (result & max_uword) | force_word;
}

uint64_t Parser::p4_expr_element()
{
	Token tk;
	uint16_t value;

	stream.read(tk);

	switch (tk.type) {
		case symbol_ref:
			if (tk.value[0] == '.')  /* local label */
				localise_symbol(tk);

			if (sym.get(tk.value, sym_anynum, value) != 0)
				return value;
			else
				if (pass == 1)
					return emit.get_loc();
				else
					error(format("Symbol {} not found", tk.value));
		case literal_chr:
			return (unsigned char) tk.value[0];
		case literal_dec:
			return p5_dec(tk.value.c_str());
		case literal_hex:
			return p5_hex(tk.value.c_str());
		case literal_bin:
			return p5_bin(tk.value.c_str());
		case '<':
			return p4_expr_element() & 0xFF;
		case '>':
			return (p4_expr_element()) >> 8 & 0xFF;
		case '^':
			return (p4_expr_element() & 0xFF) << 8;
		case '*':
			return emit.get_loc();
		default:
			error("Invalid expression");
			return (uint16_t) -1;
	}
}

uint64_t Parser::p5_dec(char const *text) {
	uint64_t value = 0;
	uint8_t c;
	char const *p;

	for (p = text; (c = *p) != 0; ++p) {
		if (!isdigit(c))
			error(std::format("Invalid decimal number {}", text));

		value *= 10;

		value += (c - '0');
	}

	return value;
}

uint64_t Parser::p5_bin(char const *text)
{
	uint64_t value = 0;
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
				error(std::format("Invalid binary number {}", text));
		}
	}

	return value;
}

uint64_t Parser::p5_hex(char const *text)
{
	uint64_t value = 0;
	char const *p;
	uint8_t c;
	uint8_t x;

	for (p = text; (c = *p) != 0; ++p) {
		if (!isxdigit(c))
			error(std::format("Invalid hex number {}", text));

		value <<= 4;
		
		x = (c >= 'A') ? (c - 'A' + 10) : (c - '0');

		value += x;
	}

	return value;
}

/*[[noreturn]] void Parser::error(string const &txt)
{
	string err = format("{}: {}\n{}\n",
		stream.get_location(),
		txt, stream.get_line_text());

	abort_msg(err.c_str());
}

[[noreturn]] void Parser::error_fmt(format_string<Args...> fmt, Args &&... args)
{
	string msg = vformat(fmt.get(), make_format_args(args...));

	error(msg);
}
*/
