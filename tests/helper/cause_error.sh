#!/bin/sh

T="$1"
E="$2"

$UA $T.s $T.bin $T.sym 2>$T.err
R=$?

EC=0
if [ $R -eq 0 ]
then
	EC=1
fi

if [ `cat $T.err | fgrep "$E" | wc -l` -eq 0 ]
then
	EC=1
fi

rm -f $T.bin $T.sym $T.err

exit $EC
