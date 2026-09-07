/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#ifndef BUFFER_H
#define BUFFER_H

#include "nfile.h"

#define _buf_line_max 255

typedef struct {
	nfile_t *nfile;
	int replay;
	char *ptr;
	int eof;
	char line[_buf_line_max + 1];
} buffer_ref;

#define buf_eof -1

extern int buf_get_next(buffer_ref *buffer);
extern void buf_rewind_1(buffer_ref *buffer);
extern void buf_rewind(buffer_ref *buffer);
extern void buf_init(buffer_ref *buffer, nfile_t *nfile);

#endif
