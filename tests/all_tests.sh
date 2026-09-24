#!/bin/sh

#####

# the test cases
ALL=`cat <<EOL
valid_instructions
macro_nested
local_labels
duplicate_labels
literals
align
input_file_not_found
output_file_error
symbol_file_error
align0
EOL
`

# the program we are testing
UA=../uasm
export UA

# run a test for max 30 seconds, force kill if still alive after another 10
TIMEOUT=30
AGONY=10

#####

echo Test cases

fail=false
for test in $ALL
do
	echo -n "  " $test
	
	if timeout -k $AGONY $TIMEOUT sh ./$test.sh
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
