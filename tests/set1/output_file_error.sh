#!/bin/sh

T=`basename $0 .sh`
E=":"

min_source.sh $T.bin
chmod 400 $T.bin

cause_error.sh "$T" "$E"
