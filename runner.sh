#!/bin/bash

# tool for compiling and running the testcases of the exercises.

# optional args + defaults
VERBOSE="false"
TEST_NUM="1"

if [[ -z "$1" || -z "$2" || -z "$3" ]]; then
    echo "usage: $0 <testcases directory> <name of binary> <name of sourcefile>"
    exit 1
fi

# required args
TESTCASES_DIR="$1"
EXEC="$2"
SRC="$3"

if [ ! -z "$4" ]; then
    TEST_NUM=$4;
fi

if [[ "$5" == "--verbose" ]]; then
    VERBOSE="true";
fi

RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
RST='\033[0m'

echo "compiling..."
g++ -Wall -g -o ${EXEC} ${SRC}
if [[ $? != 0 ]]; then
    echo "compilation failed"
    exit $?
fi

echo "running tests..."
for i in $(seq 1 ${TEST_NUM})
do
    diff ${TESTCASES_DIR}/output${i}.txt <(cat ${TESTCASES_DIR}/input${i}.txt | ./${EXEC}) > /dev/null
    if [[ $? != 0 ]]; then
        echo -e "Test ${i} ${RED}failed${RST}"
        if [[ $VERBOSE == "true" ]]; then
            echo -e "${YELLOW}Output:${RST}"
            cat ${TESTCASES_DIR}/input${i}.txt | ./${EXEC}
            echo -e "${YELLOW}Expected output:${RST}"
            cat ${TESTCASES_DIR}/output${i}.txt
        fi
        echo "start debugging?[y/n]"
        read answer
        if [[ $answer == "y" || $answer == "Y" ]]; then
            #gdb -ex "b main" -ex "run < ${TESTCASES_DIR}/input${i}.txt" ${EXEC}
            gdbgui "${EXEC} < ${TESTCASES_DIR}/input${i}.txt"
            exit 1
        fi
    else
        echo -e "Test ${i} ${GREEN}passed${RST}"
    fi
done
