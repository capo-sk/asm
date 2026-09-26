#!/bin/sh


#####

# the program we are testing
UA=`pwd`/../uasm
export UA

PATH=`pwd`/helper:$PATH

# run a test for max 30 seconds, force kill if still alive after another 10
TIMEOUT=30
AGONY=10


##### START

echo Test cases

fail=false


##### SET 1

# Test set 1: not in set 2 or 3
SET1=set1/*.sh

for testsh in $SET1
do
	test=`basename $testsh .sh`

	echo -n "  " $test
	
	if timeout -k $AGONY $TIMEOUT sh -c "cd set1 && sh ./$test.sh"
	then
		echo : pass
	else
		echo : FAIL
		fail=true
	fi
done


##### SET 2

# Set 2: test cases where we assemble a .s and check if successful
SET2=set2/*.s

for testasm in $SET2
do
	test=`basename $testasm .s`

	echo -n "  " $test
	
	if timeout -k $AGONY $TIMEOUT sh -c "cd set2 && successful_run.sh $test"
	then
		echo : pass
	else
		echo : FAIL
		fail=true
	fi
done


##### SET 3

# Set 3: test cases where we assemble a .s and compare with expected binary
SET3=set3/*.s

for testasm in $SET3
do
	test=`basename $testasm .s`

	echo -n "  " $test
	
	if timeout -k $AGONY $TIMEOUT sh -c "cd set3 && compare_with_ref.sh $test"
	then
		echo : pass
	else
		echo : FAIL
		fail=true
	fi
done


##### SET 4

# Set 4: test cases where we cause and error and check the error message

IFS=,
while read test msg
do
	echo -n "  " $test
	
	if timeout -k $AGONY $TIMEOUT sh -c "cd set4 && cause_error.sh $test \"$msg\""
	then
		echo : pass
	else
		echo : FAIL
		fail=true
	fi
done <set4/set4.csv


##### CONCLUSION

echo
if $fail
then
	echo Test suite FAILED
	exit 1
else
	echo Test suite passed
	exit 0
fi
