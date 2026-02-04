#!/bin/bash

#SECTION - global variables

DIR_OUTPUT="output"
TEMP="$DIR_OUTPUT""/temp.txt"
ERRORS="$DIR_OUTPUT""/errors.txt"
COL_ERROR="\033[31m"
COL_OK="\033[32m"
COL_TITLE="\033[33m"
RESET="\033[0m\n#------------------------------------#\n"
VALGRIND=(valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes)

#SECTION - run_test

#1)	numero test
#2) path eseguibile
#N)	argomenti
run_test()
{
	mkdir -p "$DIR_OUTPUT"
	local test_num="$1"
	local exec_path="$2"
	shift 2
	printf "$COL_TITLE""test number %d""$RESET" "$test_num"
	#"${VALGRIND[@]}"
	echo "$exec_path" "$@"
	"$exec_path" "$@" > "$TEMP" 2> /dev/null
	local exit_code="$?"
	if [ $exit_code -eq 0 ];then
		printf "$COL_OK""Success!""$RESET"
	else
		printf "$COL_ERROR""Failed with exit code %d""$RESET" $exit_code
		printf "ERROR %d:\n\n" $test_num >> errors.txt
		cat "$TEMP" >> "$ERRORS"
	fi
}
