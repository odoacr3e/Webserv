#!/bin/bash

source srcs_bash/run_test.sh

#SECTION - global variables
CMD="build/conf"
TEST_DIR="tester/tests/conf"
TEST_DIR_LOCAL="tests/conf"
OUT_DIR_LOCAL="output"
counter=0
all=("$TEST_DIR_LOCAL"/*.conf)
all=("${all[@]##*/}")
#SECTION - run
run()
{
	run_test "$counter" "$CMD" "$TEST_DIR/""$1" "$2"
	((counter++))
}

#SECTION - bash code

rm -f "$OUT_DIR_LOCAL/errors.txt"
rm -f "$OUT_DIR_LOCAL/temp.txt"
if [[ "$1" == "all" ]];then
	for conf in "${all[@]}"; do
		run "$conf"
	done
	exit
fi
if [[ "$1" == "update" ]];then
	for conf in "${all[@]}"; do
		run "$conf" "$1"
	done
	exit
fi

#SECTION - list test

run miss1.conf