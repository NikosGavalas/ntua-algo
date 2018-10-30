#!/bin/bash

PARTICLES_TESTCASES_DIR="particles_testcases"
TEST_NUM="1"
VERBOSE="true"

if [ ! -z "$1" ]; then
    TEST_NUM=$1;
fi

RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
RST='\033[0m'

echo "compiling..."
g++ -g -o particles particles.cpp
if [[ $? != 0 ]]; then
    echo "compilation failed"
    exit $?
fi

echo "running tests..."
for i in $(seq 1 ${TEST_NUM})
do
    diff ${PARTICLES_TESTCASES_DIR}/output${i}.txt <(cat ${PARTICLES_TESTCASES_DIR}/input${i}.txt | ./particles) > /dev/null
    if [[ $? != 0 ]]; then
        echo -e "Test ${i} ${RED}failed${RST}"
        if [[ $VERBOSE == "true" ]]; then
            echo -e "${YELLOW}Output:${RST}"
            cat ${PARTICLES_TESTCASES_DIR}/input${i}.txt | ./particles
            echo -e "${YELLOW}Expected output:${RST}"
            cat ${PARTICLES_TESTCASES_DIR}/output${i}.txt
        fi
        echo "start debugging?[y/n]"
        read answer
        if [[ $answer == "y" || $answer == "Y" ]]; then
            gdb -ex "run < ${PARTICLES_TESTCASES_DIR}/input${i}.txt" particles
            exit 1
        fi
    else
        echo -e "Test ${i} ${GREEN}passed${RST}"
    fi
done
