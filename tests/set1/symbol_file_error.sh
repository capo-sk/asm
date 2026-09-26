#!/bin/sh

T=`basename $0 .sh`
E=":"

min_source.sh $T.sym
chmod 400 $T.sym

cause_error.sh "$T" "$E"
