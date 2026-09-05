/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef TKSTREAM_H
#define TKSTREAM_H

#include "buffer.h"
#include "token.h"
#include "types.h"

typedef struct {
	buffer_ref *buf;
	token_context ctx;
	token latest;
	bool reuse;
} tkstream;

void ts_init(tkstream *st, buffer_ref *buf);

int ts_read(tkstream *st, token *tk);

void ts_back(tkstream *st);

void ts_rewind(tkstream *st);

#endif
