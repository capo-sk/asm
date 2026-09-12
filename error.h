/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus) || defined (__STDC_VERSION__) && __STDC_VERSION__ >= 202311L

/* C++11 / C23 syntax */

[[noreturn]] extern void abort_now();
[[noreturn]] extern void abort_msg(char const *msg);
[[noreturn]] extern void abort_fmt(char const *fmt, ...);
[[noreturn]] extern void abort_sys(char const *text);

#else

/* C11-C17 syntax */
 
#include <stdnoreturn.h>
extern noreturn void abort_now();
extern noreturn void abort_msg(char const *msg);
extern noreturn void abort_fmt(char const *fmt, ...);
extern noreturn void abort_sys(char const *text);

#endif

#ifdef __cplusplus
}
#endif

#endif
