#!/bin/bash

PARTICLES_TESTCASES_DIR="particles_testcases"

# compile
g++ -o particles particles.cpp

# run tests
for i in {1..26}
do
    cat ${PARTICLES_TESTCASES_DIR}/input${i}.txt | particles 
done
