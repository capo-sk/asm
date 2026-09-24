#!/bin/sh

T=`basename $0 .sh`
E="Argument count mismatch"

sh cause_error.sh "$T" "$E"
