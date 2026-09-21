#!/bin/sh

set -e

A=../asm
T=macro_nested
T1=${T}_macro
T2=${T}_expanded

$A $T1.s $T1.bin $T1.sym
$A $T2.s $T2.bin $T2.sym

cmp -s $T1.bin $T2.bin
R=$?

rm -f $T1.bin $T1.sym $T2.bin $T2.sym

exit $R
