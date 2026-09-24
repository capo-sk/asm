#!/bin/sh

T=`basename $0 .sh`
E=":"

sh min_source.sh $T.bin
chmod 400 $T.bin

sh cause_error.sh "$T" "$E"
