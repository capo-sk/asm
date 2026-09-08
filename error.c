/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

noreturn void abort() {
	exit(EXIT_FAILURE);
}

noreturn void error_msg(char const *msg) {
	fprintf(stderr, "%s\n", msg);
	abort();
}

noreturn void error_fmt(char const *fmt, ...) {
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	abort();
}

noreturn void error_sys(char const *text) {
	perror(text);
	abort();
}

