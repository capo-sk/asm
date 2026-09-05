/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

/* debug */

#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG

void do_debug_var(char const *_name, char const *_type, void const *_value);
#define debug_var(var, type) do_debug_var(#var, #type, (void const *)&var)
#define DV(var, type) debug_var(var, type);

void do_debug_msg(char const *_msg);
#define debug_msg(msg) do_debug_msg(#msg)
#define DM(msg) debug_msg(msg);

#else

void dont_debug(void);
#define debug_var(var, type) dont_debug()
#define DV(var, type) dont_debug();
#define debug_msg(msg) dont_debug()
#define DM(msg) dont_debug();

#endif /* NDEBUG */

#endif /* DEBUG_H */
