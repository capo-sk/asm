/* BUFFER */

#include "buffer.h"

int buf_get_next_byte(buffer_ref *buffer) {
	int byte;

	if (buffer->pointer < buffer->size) {
		byte = (int) (buffer->data[buffer->pointer]);
		++(buffer->pointer);
	} else
		byte = buf_eof;

	return byte;
}

char buf_get_next_char(buffer_ref *buffer) {
	return (char) buf_get_next_byte(buffer);
}

void buf_rewind_1(buffer_ref *buffer) {
	if (buffer->pointer != 0)
		--(buffer->pointer);
}

void buf_rewind(buffer_ref *buffer) {
	buffer->pointer = 0;
}

void buf_init(buffer_ref *buffer, u8 *content, u16 len) {
	buffer->data = content;
	buffer->size = len;
	buffer->pointer = 0;
}
