/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef PARSE_HPP
#define PARSE_HPP

#include "errorpro.hpp"
#include "buffer.hpp"
#include "tkstream.hpp"
#include "symtable.hpp"
#include "emit.hpp"
#include "macro.hpp"
#include <format>

class Parser : public ErrorProvider {
private:
	TokenStream stream;
	Token first;
	Token main_label;
	SymbolTable &sym;
	MacroTable macros;
	MacroText *current_macro;
	Emitter &emit;
	unsigned pass;
	unsigned uniq;

	void p0_source();

	int p1_line();
	void p2_labeldef();
	
	int p2_instruction();

	int p2_pseudo_instruction();
	void p3_pseudo_word();
	void p3_pseudo_byte();
	int p3_include();
	void p3_macro_header();
	void p3_pseudo_align();

	int p2_invoke_macro();

	int p2_vardef();

	int p2_location();

	uint32_t p3_expression();
	uint64_t p4_expr_element();
	uint64_t p5_dec(char const *text);
	uint64_t p5_hex(char const *text);
	uint64_t p5_bin(char const *text);

	int p2_macro_line();

	int expect_newline();
	int pushback_and_newline();

	void localise_symbol(Token &tk);
	//void make_local_label(char *local_label, char const *global_context, char const *local_part);

//	friend Emitter;  // uses error methods

public:
	Parser(Buffer &in_buf, Emitter &emitter, SymbolTable &symtable);

	void first_pass(void);
	void second_pass(void);

	[[noreturn]] virtual void error(std::string const &txt);
	//[[noreturn]] virtual void error_fmt(std::format_string<Args...> fmt, Args &&... args);
};

#endif
