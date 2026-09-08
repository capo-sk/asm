/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include <cstring>
#include "error.h"
#include "nfile.hpp"
#include "srcfile.hpp"
#include "buffer.hpp"
#include "emit.hpp"
#include "parse.h"

int main(int argc, char *argv[]) {
	char *input_filename, *output_filename, *symbol_filename;

	if (argc < 2)
		abort_fmt("Usage: %s <source.s> [<binary> [<symbols>]]", argv[0]);

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

	SrcFileList srcl;
	srcl.Add(input_filename);

	Buffer in_buf(srcl);

	NFile output_nfile(output_filename, "wb");

	Emitter emit(output_nfile);

	NFile symbol_nfile;
	int symdump_format;
	if (symbol_filename != NULL) {
		symbol_nfile = NFile(symbol_filename, "w");
		symdump_format = 1;
	} else {
		symbol_nfile = NFile(2);
		symdump_format = 0;
	}

	first_pass(in_buf);

	sym_dump(symbol_nfile, symdump_format);

	second_pass(in_buf);

	return 0;
}
