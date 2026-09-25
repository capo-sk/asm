#!/bin/sh

#####

### Test set 1: not in set 2 or 3
SET1=`cat <<EOL
valid_instructions
macro_nested
banner
output_file_error
symbol_file_error
EOL
`

### Set 2: test cases where we assemble a .s and check if successful
SET2=`cat <<EOL
local_labels
EOL
`

### Set 3: test cases where we assemble a .s and compare with expected binary
SET3=`cat <<EOL
align
literals
promote_zp
EOL
`

### Set 4: test cases where we cause and error and check the error message
SET4=`cat <<EOL
duplicate_labels
input_file_not_found
align0
div0
macro_mismatch
macro_recursive
EOL
`

### the program we are testing
UA=../uasm
export UA

# run a test for max 30 seconds, force kill if still alive after another 10
TIMEOUT=30
AGONY=10

#####

echo Test cases

fail=false

for test in $SET1
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

for test in $SET2
do
	echo -n "  " $test
	
	if timeout -k $AGONY $TIMEOUT sh ./successful_run.sh "$test"
	then
		echo : pass
	else
		echo : FAIL
		fail=true
	fi
done

for test in $SET3
do
	echo -n "  " $test
	
	if timeout -k $AGONY $TIMEOUT sh ./compare_with_ref.sh "$test"
	then
		echo : pass
	else
		echo : FAIL
		fail=true
	fi
done

IFS=,
while read test msg
do
	echo -n "  " $test
	
	if timeout -k $AGONY $TIMEOUT sh ./cause_error.sh "$test" "$msg"
	then
		echo : pass
	else
		echo : FAIL
		fail=true
	fi
done <set4.csv

echo
if $fail
then
	echo Test suite FAILED
	exit 1
else
	echo Test suite passed
	exit 0
fi
