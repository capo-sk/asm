/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8;
typedef char i8;

typedef unsigned short int u16;
typedef short int i16;

typedef unsigned u32;
typedef int i32;

#ifndef __cplusplus
typedef int bool;
#define false 0
#define true 1
#endif

#endif
