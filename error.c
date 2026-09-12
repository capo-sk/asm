/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

noreturn void abort_now() {
	exit(EXIT_FAILURE);
}

noreturn void abort_msg(char const *msg) {
	fprintf(stderr, "%s\n", msg);
	abort_now();
}

noreturn void abort_fmt(char const *fmt, ...) {
	va_list args;

	char *fmtnl = malloc(strlen(fmt) + 2);
	strcpy(fmtnl, fmt);
	strcat(fmtnl, "\n");

	va_start(args, fmt);
	vfprintf(stderr, fmtnl, args);
	va_end(args);

	abort_now();
}

noreturn void abort_sys(char const *text) {
	perror(text);
	abort_now();
}

