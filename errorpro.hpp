/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef ERRORPRO_HPP
#define ERRORPRO_HPP

class ErrorProvider {
public:
	[[noreturn]] virtual void error(char const *txt) = 0;
	[[noreturn]] virtual void error_fmt(char const *fmt, ...) = 0;
};

#endif
