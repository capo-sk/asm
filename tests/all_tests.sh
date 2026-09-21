#!/bin/sh

ALL=valid_instructions


echo Test cases

fail=false
for test in $ALL
do
	echo -n "  " $test
	if sh ./$test.sh
	then
		echo : pass
	else
		echo : FAIL
		fail=true
	fi
done

echo
if $fail
then
	echo Test suite FAILED
	exit 1
else
	echo Test suite passed
	exit 0
fi
