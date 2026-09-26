#!/bin/sh

F="$1"

if [ ! -e $F ]; then
	cat <<EOF >$F
* = 12345
.BYTE 42
EOF
fi
