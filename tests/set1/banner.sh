#!/bin/sh

T=`basename $0 .sh`
E="United Assembler"

$UA >$T.err 2>&1
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

rm -f $T.err

exit $EC
