#!/bin/bash

BOLD=$(tput bold)
NORMAL=$(tput sgr0)

echo "${BOLD}=================================================================================================${NORMAL}"
echo "${BOLD}Begin compiling nest_msa.cpp${NORMAL}"
g++ nest_msa.cpp -c
echo "${BOLD}End of compilation for nest_msa.cpp${NORMAL}"
echo "(If there was a compilation error, none of the test cases will pass)"
echo "${BOLD}=================================================================================================${NORMAL}"
echo
echo

for filename in ./tests/*; do
    echo "${BOLD}=================================================================================================${NORMAL}"
    echo "${BOLD}Testing $(basename "$filename" .cpp)()${NORMAL}"
    echo "${BOLD}-------------------------------------------------------------------------------------------------${NORMAL}"
    g++ -std=c++11 -Wall -o test tests-main.o nest_msa.o "$filename"
    ./test
    rm test
    echo "${BOLD}End of tests for $(basename "$filename" .cpp)()${NORMAL}"
    echo "${BOLD}=================================================================================================${NORMAL}"
    echo
    echo
done

rm nest_msa.o
