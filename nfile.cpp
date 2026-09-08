/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "nfile.hpp"
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include "error.h"

[[noreturn]] static void _fail_msg(char const *msg) {
	abort_msg(msg);
}

[[noreturn]] static void _fail_name(char const *name) {
	abort_sys(name);
}

[[noreturn]] void NFile::_fail() {
	_fail_name(name);
}

NFile::NFile(int which) {
	char const *tname;

	switch (which) {
		case 0:
			file = stdin;
			tname = "<stdin>";
			break;
		case 1:
			file = stdout;
			tname = "<stdout>";
			break;
		case 2:
			file = stderr;
			tname = "<stderr>";
			break;
		default:
			_fail_msg("Invalid standard file number");
	}
	name = strdup(tname);
}

NFile::NFile(char const *name, char const *mode) {
	file = fopen(name, mode);
	if (file != NULL)
		name = strdup(name);
	else
		_fail_name(name);
}

NFile::NFile() {
	file = NULL;
	name = strdup("");
}

NFile::~NFile() {
	int result;

	if (name[0] != '<' && name[0] != 0)
		result = fclose(file);
	else  // do not close stdin/out/err
		result = 0;

	if (result == 0)
		free(name);
	else
		_fail();
}

void NFile::putc(unsigned char c) {
	fputc(c, file);
	if (ferror(file))
		_fail();
}

void NFile::printf(char const *fmt, ...) {
	va_list params;

	va_start(params, fmt);
	vfprintf(file, fmt, params);
}

unsigned NFile::getline(char *line, unsigned max) {
	void *result;

	if (feof(file))
		return 0;

	result = fgets(line, max, file);
	if (result == NULL && ferror(file))
		_fail();
	else
		return strlen(line);
}

void NFile::rewind() {
	::rewind(file);
}
