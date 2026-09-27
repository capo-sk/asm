/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "error.hpp"
#include <iostream>

// yes, this is a global variable
// no point having a pointer to the same thing in all objects
static Buffer const *source;

void set_error_source(Buffer const &src)
{
	source = &src;
}

// issue an error with context
[[noreturn]] void error(string const &txt)
{
	string err;
	
	if (source)
		err = format("{}: {}\n{}\n", source->get_location(), txt, source->get_line_text());
	else
	 	err = format("ERROR: {}", txt);

	abort_msg(err);
}

[[noreturn]] void abort_now()
{
	exit(EXIT_FAILURE);
}

[[noreturn]] void abort_msg(string const &msg)
{
    std::cerr << msg << "\n";
	abort_now();
}

[[noreturn]] void abort_sys(string const &text)
{
	perror(text.c_str());
	abort_now();
}