#!/bin/sh

T="$1"

$UA $T.s $T.bin $T.sym
 
DEL_REF=
if [ ! -e $T.ref ]
then
	./make_ref $T.s $T.ref
	DEL_REF=$T.ref
fi

cmp -s $T.bin $T.ref
R=$?

rm -f $T $T.bin $T.sym $DEL_REF

exit $R
