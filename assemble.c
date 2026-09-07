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
#include "symtable.h"
#include "nfile.h"

static u8 input_buffer[65536];

int main(int argc, char **argv) {
	buffer_ref in_buf;
	size_t file_len;
	char *input_filename, *output_filename, *symbol_filename;
	nfile_t *input_nfile, *output_nfile, *symbol_nfile;
	const char usage_message[] = "Usage: asm <source.s> [<binary> [<symbols>]]";
	const char generic_error_message[] = "Error";
	int symdump_format;

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
	else
		symbol_filename = NULL;

/*
	printf("Input: %s\n", input_filename);
	printf("Output: %s\n", output_filename);
	if (symbol_filename != NULL)
		printf("Symbols: %s\n", symbol_filename);
*/

	input_nfile = nfopen(input_filename, "r");
/*
	file_len = nfread(input_nfile, input_buffer, sizeof(input_buffer) - 1);
*/

	buf_init(&in_buf, input_nfile);

	output_nfile = nfopen(output_filename, "wb");

	emit_init(output_nfile);

	if (symbol_filename != NULL) {
		symbol_nfile = nfopen(symbol_filename, "w");
		symdump_format = 1;
	} else {
		symbol_nfile = nfstdout();
		symdump_format = 0;
	}

	first_pass(&in_buf);

	sym_dump(symbol_nfile, symdump_format);

	nfclose(symbol_nfile);

	second_pass();

	nfclose(input_nfile);
	nfclose(output_nfile);

	return 0;
}
