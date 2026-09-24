/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef ERRORPRO_HPP
#define ERRORPRO_HPP

#include <string>

class ErrorProvider {
public:
	[[noreturn]] virtual void error(std::string const &msg) = 0;
	//[[noreturn]] virtual void error_fmt(char const *fmt, ...) = 0;
};

#endif
