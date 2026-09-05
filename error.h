/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef ERROR_H
#define ERROR_H

extern unsigned line;
void error(char const *msg);
void error_str(char const *fmt, char const *str);

#endif
