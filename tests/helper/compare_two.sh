#!/bin/sh

T1="$1"
T2="$2"

$UA $T1.s $T1.bin $T1.sym
$UA $T2.s $T2.bin $T2.sym

cmp -s $T1.bin $T2.bin
R=$?

rm -f $T1.bin $T1.sym $T2.bin $T2.sym

exit $R
