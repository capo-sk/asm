/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

std::string sfmt(char const *fmt, ...);
std::string vsfmt(char const *fmt, va_list params);

#endif