/* BUFFER */

#ifndef BUFFER_H
#define BUFFER_H

#include "types.h"

typedef struct {
	u16 pointer;
	u16 size;
	u8 *data;
} buffer_ref;

#define buf_eof -1

void buf_init(buffer_ref *buffer, u8 *content, u16 len);

int buf_get_next_byte(buffer_ref *buffer);

char buf_get_next_char(buffer_ref *buffer);

void buf_rewind_1(buffer_ref *buffer);

void buf_rewind(buffer_ref *buffer);

#endif
