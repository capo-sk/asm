#!/bin/sh

T=`basename $0 .sh`
E="Macro may not invoke itself"

sh cause_error.sh "$T" "$E"
