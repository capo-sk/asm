#!/bin/sh

set -e

T=valid_instructions
A=../asm

cc -o $T $T.c
./$T
rm valid_instructions
$A $T.s $T.bin $T.sym
 
cmp -s $T.bin $T.ref
R=$?

rm -f $T $T.bin $T.ref $T.sym

exit $R
