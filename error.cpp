/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "error.hpp"

#include <iostream>
#include <format>

[[noreturn]] void abort_now()
{
	exit(EXIT_FAILURE);
}

[[noreturn]] void abort_msg(string const &msg)
{
    std::cerr << msg << "\n";
	abort_now();
}

/*noreturn void abort_fmt(char const *fmt, ...)
{
	va_list args;

	char *fmtnl = malloc(strlen(fmt) + 2);
	strcpy(fmtnl, fmt);
	strcat(fmtnl, "\n");

	va_start(args, fmt);
	vfprintf(stderr, fmtnl, args);
	va_end(args);

	abort_now();
}*/

[[noreturn]] void abort_sys(string const &text)
{
	perror(text.c_str());
	abort_now();
}

// class ErrorProvider

[[noreturn]] void ErrorProvider::error(string const &txt) const
{
	string err = std::format("{}: {}\n{}\n",
		source->get_location(),
		txt,
        source->get_line_text());

	abort_msg(err);

}

// class ErrorConsumer

[[noreturn]] void ErrorConsumer::error(std::string const &msg) const
{
    if (err)
        err->error(msg);
    else
        abort_msg(string("ERROR: ") + msg);

    abort_now();  // only because GCC gets confused and claims function returns
}

void ErrorConsumer::set_error_provider(ErrorProvider const &ep)
{
	err = &ep;
}

void ErrorConsumer::clear_error_provider()
{
    err = NULL;
}

