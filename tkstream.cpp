/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/*
	This is the lexical analyser.

	It reads characters from a Buffer (wildly inadequate name), which is really the
	serialised text of the main source file + include files (expanded where invoked),
	including macro definitions, + macros expanded where invoked.

	It combines characters into tokens.

	It's all a big state machine.

	Depending on /mode/ we break up the input in tokens in different ways:
	- mode /assembly/: break it up in labels, opcodes, operands etc.
	- mode /macro/ is for macro definitions; the whole line is a token, to be stored
		for later invocation; we remove excess whitespace and convert to upper case;
		a line consisting of ".ENDM" is recognised and returned as pseudo-opcode token
	- mode /word/ is used for making tokens of macro parameters on invocation
		any amount of non-whitespace makes up a "word" token which is passed on to
		replace the positional parameters while the macro is expanded

	/read/ returns the next Token
	/pushback/ puts back the latest token to be read again
	/rewind/ goes back to the beginning of the Buffer (for a second pass)
	/reset/ is rewind + empty the list of include files, so we can include them again
		(for the second pass)
	/_get_next_.../ is where the magic happens for each of the modes

	/nested_file/ and /nested_source/ are used to divert input to an include file
	or expanded macro; they should probably be in Parser but Parser does not have
	direct access to Buffer/SrcStack, so they are here
*/

#include "tkstream.hpp"
#include "srcfile.hpp"
#include <cctype>
#include "opcodes.hpp"
#include <string>

using std::string;

Token::Token()
{
	clear();
}

void Token::clear()
{
	type = invalid;
	value.clear();
}

Token &Token::operator=(Token const &tk)
{
	type = tk.type;

	if (type == actual_opcode || type == pseudo_opcode)
		number = tk.number;
	else
	 	value = tk.value;

	return *this;
}
TokenStream::TokenStream(Buffer &in_buf) : buf(in_buf) {
	mode = assembly;
	ctx = line_start;
	reuse = false;
}

int TokenStream::read(Token &tk) {
	int retval;

	if (reuse) {
		reuse = false;
		retval = 1;
	} else {
		retval = _get_next_token_multimode(latest);
	}

	tk = latest;

	return retval;
}

void TokenStream::pushback() {
	reuse = true;
}

void TokenStream::rewind() {
	buf.rewind();
	reuse = false;
	ctx = line_start;
}

void TokenStream::reset()
{
	buf.reset();
	rewind();
}


int TokenStream::_get_next_token_multimode(Token &tk)
{
	switch (mode) {
		case assembly:
			return _get_next_token(tk);
		case macro:
			return _get_next_macro_line(tk);
		case words:
			return _get_next_word(tk);
		default:
			return -1;
	}
}

int TokenStream::_get_next_word(Token &tk)
{
	int c;
	bool incomplete_token = true;

	tk.clear();  // start with type=invalid, value=""

	while (incomplete_token) {
		c = buf.get_next();

		/* end of line or file */
		if (c == '\n' || c == Buffer::eofmark) {
			buf.pushback();
			incomplete_token = false;
			break;
		}

		/* convert to uppercase */
		c = toupper(c);

		/* whitespace */
		if (isspace(c)) {
			if (tk.value.length() == 0) {
				// ignore starting whitespace
				continue;
			} else {
				// whitespace terminates word
				incomplete_token = false;
				break;
			}
		}

		// anything else accumulates into the value
		tk.value += (char)c;
	}

	if (tk.value.length() > 0) {
		tk.type = word;
		return 1;
	} else {
		tk.type = skip;
		return 0;
	}
}

int TokenStream::_get_next_macro_line(Token &tk)
{
	int c, prev;
	bool incomplete_token = true;
	bool inside_comment = false;

	tk.clear();  // start with type=invalid, value=""

	c = 0;
	while (incomplete_token) {
		prev = c;
		c = buf.get_next();

		if (c == Buffer::eofmark) {
			if (tk.value.length() == 0) {  // reached end of file
				tk.type = endline;
				if (buf.close_file() == 0) { // no more files
					ctx = end_of_file;
					return 0;
				} else {
					ctx = line_start;
					return 1;
				}
			} else {  // EOF terminates word
				buf.pushback();
				incomplete_token = false;
				break;
			}
		}

		// end of line
		if (c == '\n') {
			// token is complete (whole line)
			incomplete_token = false;
			break;
		}

		// if we are in a comment, ignore all until newline or eof
		if (inside_comment)
			continue;

		// convert to uppercase
		c = toupper(c);

		// whitespace
		if (isspace(c)) {
			c = ' ';  // compress any amount of whitespace between words to one space
			if (tk.value.length() == 0 || prev == ' ') {
				// ignore starting space or more than one space
				continue;
			} else {
				// first space is appended
				tk.value += (char)c;
				continue;
			}
		}

		// ; starts comment
		if (c == ';') {
			// process the comment (ignore until newline)
			inside_comment = true;
			continue;
		}

		// anything else: append to the output
		tk.value += (char)c;
	}

	// remove trailing space if any
	if (tk.value.length() > 1 && tk.value[tk.value.length() - 1] == ' ')
		tk.value.erase(tk.value.length() - 1);

	// if the line is ENDM then return ENDM token, otherwise the line is the token
	const unsigned endm_opcode_idx = 1;
	if (tk.value == pseudos[endm_opcode_idx].mnemonic) {
		tk.type = pseudo_opcode;
		tk.value[0] = endm_opcode_idx;
	} else {
		tk.type = macro_line;
	}

	return 1;
}

/*
return: tk is populated with the next token
return: 1 if valid token, 0 if eof, -1 error
update: in_buf as consumed
update: ctx with new context if changed
*/
int TokenStream::_get_next_token(Token &tk)
{
	int c;
	unsigned char literal_delimiter;
	bool incomplete_token = true;
	bool inside_string_literal = false;
	bool inside_comment = false;

	tk.clear();  // start with type=invalid, value=""

	while (incomplete_token) {
		c = buf.get_next();

		if (c == Buffer::eofmark) {
			if (tk.value.length() == 0) {  // reached end of file
				tk.type = endline;
				if (buf.close_file() == 0) { // no more files
					ctx = end_of_file;
					return 0;
				} else {
					ctx = line_start;
					return 1;
				}
			} else {  // EOF terminates word
				buf.pushback();
				incomplete_token = false;
				break;
			}
		}

		// if we are inside a string literal, accumulate until we find the end delimiter
		if (inside_string_literal) {
			if (c == literal_delimiter) {
				if (literal_delimiter == '"')
					tk.type = literal_str;
				else
					tk.type = literal_chr;
				incomplete_token = false;
				break;
			} else {
				tk.value += (char)c;
				continue;
			}
		}

		// if we are in a comment, ignore all until newline
		if (inside_comment) {
			if ('\n') {  // newline ends comment
				tk.type = endline;
				ctx = line_start;
				return 1;
			}
			continue;
		}

		if (tk.value.length() == 0) {
			// start char/string literal 'c' "str"
			if (c == '"' || c == '\'') {
				literal_delimiter = c;
				inside_string_literal = true;
				continue;
			}

			// start comment
			if (c == ';') {
				inside_comment = true;
				continue;
			}

			// ignore starting whitespace
			if (isspace(c))
				continue;

			// colon may not start a token
			if (c == ':') {
				tk.type = invalid;
				tk.value = ':';
				ctx = line_middle;
				return 1;
			}
		}
	
		// convert to uppercase
		c = toupper(c);

		// accumulate word characters
		if (isalnum(c) || c == '_' || c == '$' || c == '%' || c == '.' || c == '&') {
			tk.value += (char)c;
			continue;
		}

		// colon signals end of label
		if (c == ':') {
			tk.type = label_def;
			ctx = line_middle;
			return 1;
		}
	
		// anything else (special symbols, space, return etc.) terminates the token
		// if first character, the symbol is the token
		if (tk.value.length() == 0) {
			// c is the token
			if (c == '\n') {
				tk.type = endline;
				ctx = line_start;
			} else {
					tk.type = (token_type)c;
					ctx = line_middle;
			}
			return 1;
		} else {		/* c is not part of the token */
			buf.pushback();
			incomplete_token = false;
			break;
		}
	}

	if (tk.type == invalid) {
		c = tk.value[0];
		if (c == '$') {
			tk.type = literal_hex;
			tk.value.erase(0, 1);
		} else if (c == '%') {
			tk.type = literal_bin;
			tk.value.erase(0, 1);
		} else if (isdigit(c))
			tk.type = literal_dec;
		else if (c == '&') {
			tk.type = macro_ref;
			tk.value.erase(0, 1);
		} else {
			unsigned i;

			// is it a register?
			for (i = 0; i < num_registers; ++i) {
				if (tk.value == cpu_registers[i]) {
					tk.type = cpu_register;
					goto found;
				}
			}

			// is it an opcode?
			for (i = 0; i < num_opcodes; ++i) {
				if (tk.value == opcodes[i].mnemonic) {
					tk.type = actual_opcode;
					tk.number = i;
					goto found;
				}
			}

			// is it a pseudo-opcode?
			for (i = 0; i < num_pseudos; ++i) {
				if (tk.value == pseudos[i].mnemonic) {
					tk.type = pseudo_opcode;
					tk.number = i;
					if (i == pseudo_macro)
						ctx = macro_header1;
					goto found;
				}
			}

			// what type of identifier is it?
			if (ctx == line_start) {
				tk.type = var_def;
			} else if (ctx == macro_header1) {
				tk.type = macro_def;
				ctx = macro_header2;
			} else if (ctx == macro_header2) {
				tk.type = macro_par;
			} else {
				tk.type = symbol_ref;
			}
		}
	}

found:

	if (ctx == line_start)
		ctx = line_middle;

	return 1;
}

std::string TokenStream::get_location()
{
	return buf.get_location();
}

void TokenStream::nested_file(char const *name)
{
	if (!buf.is_present(name)) {
		SrcFile *f = new SrcFile(name);
		buf.new_source_once(f);
	}
}

void TokenStream::set_mode(token_mode a_mode)
{
	mode = a_mode;
}

string TokenStream::get_line_text()
{
	return buf.get_line_text();
}

SrcText &TokenStream::get_current()
{
	return buf.get_current();
}

void TokenStream::nested_source(SrcText &source)
{
	buf.new_source(&source);
}
