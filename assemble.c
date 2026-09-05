#include "buffer.h"
#include "parse.h"
#include <stdio.h>


static u8 input_buffer[8192];

int main(void) {
	buffer_ref in_buf;
	int file_len;

	file_len = fread(input_buffer, 1, sizeof(input_buffer) - 1, stdin);
	buf_init(&in_buf, input_buffer, file_len);

	first_pass(&in_buf);
	second_pass();

	return 0;
}

