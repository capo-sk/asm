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

class Parser : public ErrorProvider {
private:
	TokenStream stream;
	Token first;
	Token main_label;
	SymbolTable &sym;
	Emitter &emit;
	unsigned pass;

	void do_pass();
	int do_line();
	int do_opcode();
	int do_pseudo();
	void do_pseudo_word();
	void do_pseudo_byte();
	int do_macrodef();
	int do_macro();
	void do_labeldef();
	int do_vardef();
	int do_location();
	u32 parse_expr();
	u16 expr_element();
	u16 parse_dec(char const *text);
	u16 parse_hex(char const *text);
	int expect_newline();
	void make_local_label(char *local_label, char const *global_context, char const *local_part);
	int do_include();

	friend Emitter;  // uses error methods

public:
	Parser(Buffer &in_buf, Emitter &emitter, SymbolTable &symtable);

	void first_pass(void);
	void second_pass(void);

	[[noreturn]] virtual void error(char const *txt);
	[[noreturn]] virtual void error_fmt(char const *fmt, ...);
};

#endif
