#!/bin/sh

set -e

T=valid_instructions

cc -o $T $T.c
./$T
rm valid_instructions
../asm $T.s $T.bin $T.sym
 
cmp -s $T.bin $T.ref
R=$?

rm -f $T $T.bin $T.ref

exit $R
