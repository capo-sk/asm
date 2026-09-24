#!/bin/sh

T=`basename $0 .sh`
E="Division by zero"

sh cause_error.sh "$T" "$E"
