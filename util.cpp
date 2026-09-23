/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "util.hpp"
#include <cstdarg>
#include <cstdio>

static const unsigned buffer_size = 1024;

std::string vsfmt(char const *fmt, va_list params)
{
    char buffer[buffer_size];

    vsnprintf(buffer, buffer_size, fmt, params);
    buffer[buffer_size - 1] = 0;  // in case the output is buffer_size or longer

    return std::string(buffer);
}

std::string sfmt(char const *fmt, ...)
{
    char buffer[buffer_size];
    va_list params;

    va_start(params, fmt);
    vsnprintf(buffer, buffer_size, fmt, params);
    buffer[buffer_size - 1] = 0;  // in case the output is buffer_size or longer
    va_end(params);

    return std::string(buffer);
}
