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

	int p2_invoke_macro();

	int p2_vardef();

	int p2_location();

	u32 p3_expression();
	u16 p4_expr_element();
	u16 p5_dec(char const *text);
	u16 p5_hex(char const *text);

	int p2_macro_line();

	int expect_newline();
	int pushback_and_newline();

	void localise_label(Token &tk);
	//void make_local_label(char *local_label, char const *global_context, char const *local_part);

//	friend Emitter;  // uses error methods

public:
	Parser(Buffer &in_buf, Emitter &emitter, SymbolTable &symtable);

	void first_pass(void);
	void second_pass(void);

	[[noreturn]] virtual void error(char const *txt);
	[[noreturn]] virtual void error_fmt(char const *fmt, ...);
};

#endif
