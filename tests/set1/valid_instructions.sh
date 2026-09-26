#!/bin/sh

T=`basename $0 .sh`

cc -o $T $T.c
./$T
rm valid_instructions

compare_with_ref.sh "$T"
