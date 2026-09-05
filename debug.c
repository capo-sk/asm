/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* debug */

#include <string.h>
#include <ctype.h>
#include "debug.h"
#include <stdio.h>
#include "types.h"

const char *types[] =
	{ "i8", "u8", "i16", "u16", "i32", "u32", "ptr", "str", "bool" };

void do_debug_var(char const *_name, char const *_type, void const *_value) {
	unsigned i;
	unsigned len = sizeof(types) / sizeof(char *);

	for (i = 0; i < len; ++i) {
		if (strcmp(types[i], _type) == 0)
			break;
	}

	switch (i) {
		case 0:	/* i8 */
			if (isprint((char)*(i8 const *)_value))
				printf("VAR %s (%s) = %d $%02X '%c'\n", _name, _type, (int)*(i8 const *)_value, (int)*(i8 const *)_value, (char)*(i8 const *)_value);
			else
				printf("VAR %s (%s) = %d $%02X\n", _name, _type, (int)*(i8 const *)_value, (int)*(i8 const *)_value);
			break;
		case 1:	/* u8 */
			if (isprint((char)*(i8 const *)_value))
				printf("VAR %s (%s) = %u $%02X '%c'\n", _name, _type, (unsigned)*(u8 const *)_value, (unsigned)*(u8 const *)_value, (char)*(u8 const *)_value);
			else
				printf("VAR %s (%s) = %u $%02X\n", _name, _type, (unsigned)*(u8 const *)_value, (unsigned)*(u8 const *)_value);
			break;
		case 2:	/* i16 */
			printf("VAR %s (%s) = %d $%04X\n", _name, _type, (int)*(i16 const *)_value, (int)*(i16 const *)_value);
			break;
		case 3:	/* u16 */
			printf("VAR %s (%s) = %u $%04X\n", _name, _type, (unsigned)*(u16 const *)_value, (unsigned)*(u16 const *)_value);
			break;
		case 4:	/* i32 */
			printf("VAR %s (%s) = %ld $%08lX\n", _name, _type, (long)*(i32 const *)_value, (long)*(i32 const *)_value);
			break;
		case 5:	/* u32 */
			printf("VAR %s (%s) = %lu $%08lX\n", _name, _type, (unsigned long)*(u32 const *)_value, (unsigned long)*(u32 const *)_value);
			break;
		case 6:	/* ptr */
			printf("VAR %s (%s) = $%02X\n", _name, _type, (unsigned)*(void * const *)_value);
			break;
		case 7: /* str */
			printf("VAR %s (%s) = '%s'\n", _name, _type, (char const *)_value);
			break;
		case 8:	/* bool */
			printf("VAR %s (%s) = %s (%d)\n", _name, _type, *(int const *)_value ? "true" : "false", *(int const *)_value);
			break;
		default:
			printf("VAR %s (%s): unknown _type\n", _name, _type);
	}
}

void do_debug_msg(char const *_msg) {
	printf("%s\n", _msg);
}

void dont_debug(void) { }

