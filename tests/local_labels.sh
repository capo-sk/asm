#!/bin/sh

set -e

A=../asm
T=local_labels

$A $T.s $T.bin $T.sym
R=$?

rm -f $T.bin $T.sym

exit $R
