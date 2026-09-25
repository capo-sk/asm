/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include <fstream>
#include <iostream>
#include <string>
#include "error.h"
#include "buffer.hpp"
#include "emit.hpp"
#include "parse.hpp"
#include "version.h"

using namespace std;

int main(int argc, char *argv[]) {
	const ios::openmode m_r = ios::in;
	const ios::openmode m_w = ios::out | ios::trunc;
	const ios::openmode m_wb = m_w | ios_base::binary;
	string input_filename, output_filename, symbol_filename;

	if (argc < 2)
		abort_fmt("United Assembler -- version %s\n\n"
			"Usage: %s <source.s> [<binary> [<symbols>]]", version_string, argv[0]);

	input_filename = argv[1];

	if (argc > 2)
		output_filename = argv[2];
	else {
		auto pos = input_filename.rfind('.');
		if (pos != string::npos)
			output_filename = input_filename.substr(0, pos);
		else
			output_filename = input_filename + ".prg";
	}

	if (argc > 3)
		symbol_filename = argv[3];
	else
		symbol_filename = "";

	Buffer in_buf(input_filename);
	//srcl.new_source_once(new SrcFile(input_filename));
	//srcl.AdvanceLine();  // otherwise it starts from 0; can't be bothered fixing it properly

	//Buffer in_buf(srcl);

	ofstream output_file(output_filename, m_wb);
	if (!output_file)
		abort_sys(output_filename.c_str());

	Emitter emit(output_file);

	ostream *symbol_filep;
	ofstream symbol_file;
	int symdump_format;
	if (symbol_filename.length() > 0) {
		symbol_file.open(symbol_filename, m_w);
		if (!symbol_file)
			abort_sys(symbol_filename.c_str());
		symbol_filep = &symbol_file;
		symdump_format = 1;
	} else {
		symbol_filep = &std::cout;
		symdump_format = 0;
	}

	SymbolTable symtab;

	Parser parser(in_buf, emit, symtab);
	emit.set_error_provider(&parser);
	symtab.set_error_provider(&parser);

	parser.first_pass();

	symtab.dump(*symbol_filep, symdump_format);

	parser.second_pass();

	return 0;
}
