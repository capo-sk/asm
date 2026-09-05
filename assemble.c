/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "parse.h"
#include "emit.h"

static u8 input_buffer[65536];

int main(int argc, char **argv) {
	buffer_ref in_buf;
	size_t file_len;
	char *input_filename, *output_filename, *symbol_filename;
	FILE *input_file, *output_file;
	const char usage_message[] = "Usage: asm <source.s> [<binary> [<symbols>]]";
	const char generic_error_message[] = "Error";

	if (argc < 2) {
		fprintf(stderr, "%s\n\n", usage_message);
		exit(EXIT_FAILURE);
	}

	input_filename = argv[1];

	if (argc > 2)
		output_filename = argv[2];
	else {
		output_filename = strdup(input_filename);
		char *dot;
		dot = strrchr(output_filename, '.');
		if (dot != NULL) *dot = 0;
	}

	if (argc > 3)
		symbol_filename = argv[3];

	printf("Input: %s\n", input_filename);
	printf("Output: %s\n", output_filename);

	input_file = fopen(input_filename, "r");
	if (input_file == NULL) {
		perror(input_filename);
		exit(EXIT_FAILURE);
	}
	file_len = fread(input_buffer, 1, sizeof(input_buffer) - 1, input_file);
	fclose(input_file);

	buf_init(&in_buf, input_buffer, file_len);

	output_file = fopen(output_filename, "wb");
	if (output_file == NULL) {
		perror(output_filename);
		exit(EXIT_FAILURE);
	}

	emit_init(output_file);

	first_pass(&in_buf);
	second_pass();

	return 0;
}

