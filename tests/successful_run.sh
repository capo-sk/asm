#!/bin/sh

T="$1"

$UA $T.s $T.bin $T.sym
R=$?

rm -f $T.bin $T.sym

exit $R
