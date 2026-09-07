/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef NFILE_H
#define NFILE_H

#include <stdio.h>

typedef struct {
	FILE *file;
	char const *name;
} nfile_t;

/* extern nfile_t *nstdin, *nstdout, *nstderr; */
extern nfile_t *nfopen(char const *name, char const *mode);
extern void nfclose(nfile_t *nfile);
extern size_t nfread(nfile_t *nfile, void *dest, size_t max);
extern void nfputc(nfile_t *nfile, unsigned char c);
extern void nfprintf(nfile_t *nfile, char const *format, ...);
extern size_t nfgetline(nfile_t *nfile, char *output, size_t max);
extern void nfrewind(nfile_t *nfile);
extern nfile_t *nfstdin(void);
extern nfile_t *nfstdout(void);
extern nfile_t *nfstderr(void);

#endif
