/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "nfile.h"

/*
static nfile_t _nstdin, _nstdout, _nstderr;
nfile_t *nstdin = &_nstdin, *nstdout = &_nstdout, *nstderr = &_nstderr;
*/

static void _nf_fail_name(char const *name) {
	perror(name);
	exit(EXIT_FAILURE);
}

static void _nf_fail(nfile_t *nfile) {
	if (nfile == NULL) {
		fprintf(stderr, "NULL nfile object\n");
		exit(EXIT_FAILURE);
	} else
		_nf_fail_name(nfile->name);
}

static void _nf_notnull(nfile_t *nfile) {
	if (nfile == NULL)
		_nf_fail(nfile);
}

static nfile_t *_nf_std(int which) {
	nfile_t *result;
	char *name;

	result = malloc(sizeof(nfile_t));

	switch (which) {
		case 0:
			result->file = stdin;
			name = "<stdin>";
			break;
		case 1:
			result->file = stdout;
			name = "<stdout>";
			break;
		case 2:
			result->file = stderr;
			name = "<stderr>";
			break;
		default:
			/* should not be here */
	}
	result->name = strdup(name);

	return result;
}

nfile_t *nfstdin(void) {
	return _nf_std(0);
}

nfile_t *nfstdout(void) {
	return _nf_std(1);
}

nfile_t *nfstderr(void) {
	return _nf_std(2);
}

nfile_t *nfopen(char const *name, char const *mode) {
	FILE *file;

	file = fopen(name, mode);
	if (file != NULL) {
		nfile_t *nfile;
		nfile = malloc(sizeof(nfile_t));
		nfile->file = file;
		nfile->name = strdup(name);
		return nfile;
	} else
		_nf_fail_name(name);
}

void nfclose(nfile_t *nfile) {
	int result;

	_nf_notnull(nfile);

	if (nfile->name[0] != '<')
		result = fclose(nfile->file);
	else  /* do not close stdin/out/err */
		result = 0;

	if (result == 0)
		free((void *) (nfile->name));
	else
		_nf_fail(nfile);
}

size_t nfread(nfile_t *nfile, void *dest, size_t max) {
	size_t result;

	_nf_notnull(nfile);

	result = fread(dest, 1, max, nfile->file);
	if (ferror(nfile->file))
		_nf_fail(nfile);

	return result;
}

void nfputc(nfile_t *nfile, unsigned char c) {
	_nf_notnull(nfile);

	fputc(c, nfile->file);
	if (ferror(nfile->file))
		_nf_fail(nfile);
}

void nfprintf(nfile_t *nfile, char const *fmt, ...) {
	va_list params;

	_nf_notnull(nfile);

	va_start(params, fmt);
	vfprintf(nfile->file, fmt, params);
}

size_t nfgetline(nfile_t *nfile, char *line, size_t max) {
	void *result;

	_nf_notnull(nfile);

	if (feof(nfile->file))
		return 0;

	result = fgets(line, max, nfile->file);
	if (result == NULL && ferror(nfile->file))
		_nf_fail(nfile);
	else
		return strlen(line);
}

void nfrewind(nfile_t *nfile) {
	rewind(nfile->file);
}
