/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef ERROR_HPP
#define ERROR_HPP

#include "common.hpp"
#include "buffer.hpp"

void set_error_source(Buffer const &src);
[[noreturn]] void error(string const &txt);

[[noreturn]] extern void abort_now();
[[noreturn]] extern void abort_msg(string const &msg);
[[noreturn]] extern void abort_sys(string const &text);

#endif
