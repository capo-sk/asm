#!/bin/sh

T=`basename $0 .sh`

T1=${T}_macro
T2=${T}_expanded

sh compare_two.sh "$T1" "$T2"
