#!/bin/sh

T=`basename $0 .sh`
E="Duplicate symbol"

sh cause_error.sh "$T" "$E"
