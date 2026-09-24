#!/bin/sh

T=`basename $0 .sh`
E=":"

sh min_source.sh $T.sym
chmod 400 $T.sym

sh cause_error.sh "$T" "$E"
