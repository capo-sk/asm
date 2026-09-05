#include "error.h"
#include <stdio.h>
#include <stdlib.h>

unsigned line;

static void error_line(void) {
	fprintf(stderr, "Error at line %u: ", line);
}

void error_str(char const *fmt, char const *str) {
	error_line();
	fprintf(stderr, fmt, str);
	fprintf(stderr, "\n");
	exit(1);
}

void error(char const *msg) {
	error_str("%s", msg);
}
