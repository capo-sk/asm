/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* BUFFER */

#include "buffer.h"

int buf_get_next(buffer_ref *buffer) {
	if (buffer->eof)
		return buf_eof;

	if (buffer->replay > 0xff) {  /* after rewind_1 */
		buffer->replay &= 0xff;
	} else {
		if (*buffer->ptr == 0) {  /* nothing left in the line */
			if (nfgetline(buffer->nfile, buffer->line, sizeof(buffer->line)) == 0) {  /* EOF */
				buffer->eof = 1;
				return buf_eof;
			}
			buffer->ptr = buffer->line;
		}
		buffer->replay = *(unsigned char *)buffer->ptr++;
	}

	return buffer->replay;
}

void buf_rewind_1(buffer_ref *buffer) {
	buffer->replay |= 0x100;
}

void buf_rewind(buffer_ref *buffer) {
	nfrewind(buffer->nfile);
	buffer->replay = buf_eof;
	buffer->line[0] = 0;
	buffer->ptr = buffer->line;
	buffer->eof = 0;
}

void buf_init(buffer_ref *buffer, nfile_t *nfile) {
	buffer->nfile = nfile;
	buffer->replay = buf_eof;
	buffer->line[0] = 0;
	buffer->ptr = buffer->line;
	buffer->eof = 0;
}
