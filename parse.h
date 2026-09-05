/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef PARSE_H
#define PARSE_H

#include "buffer.h"

void first_pass(buffer_ref *in_buf);
void second_pass(void);

#endif
