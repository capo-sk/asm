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

	void p0_source();
	int p1_line();
	int p2_instruction();
	int p2_pseudo_instruction();
	void do_pseudo_word();
	void do_pseudo_byte();
	int p2_invoke_macro();
	void p3_labeldef();
	int p2_vardef();
	int p2_location();
	u32 parse_expr();
	u16 expr_element();
	u16 parse_dec(char const *text);
	u16 parse_hex(char const *text);
	int expect_newline();
	int rewind_and_newline();
	void make_local_label(char *local_label, char const *global_context, char const *local_part);
	int p3_include();
	void p3_macro_header();
	int p2_macro_line();

	friend Emitter;  // uses error methods

public:
	Parser(Buffer &in_buf, Emitter &emitter, SymbolTable &symtable);

	void first_pass(void);
	void second_pass(void);

	[[noreturn]] virtual void error(char const *txt);
	[[noreturn]] virtual void error_fmt(char const *fmt, ...);
};

#endif
