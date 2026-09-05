/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "tkstream.h"

void ts_init(tkstream *st, buffer_ref *buf) {
	st->buf = buf;
	st->reuse = false;
	st->ctx = line_start;
}

int ts_read(tkstream *st, token *tk) {
	int retval;

	if (st->reuse) {
		st->reuse = false;
		retval = 1;
	} else {
		retval = get_next_token(st->buf, &st->ctx, &st->latest);
	}

	*tk = st->latest;
	return retval;
}

void ts_back(tkstream *st) {
	st->reuse = true;
}

void ts_rewind(tkstream *st) {
	buf_rewind(st->buf);
	st->reuse = false;
}

