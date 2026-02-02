#!/bin/bash

source srcs_bash/run_test.sh

#SECTION - global variables
CMD="build/conf"
TEST_DIR="tests/conf"
#SECTION - run
run()
{
	run_test "$CMD" "$TEST_DIR/""$1"
}

#SECTION - test list

run test.conf