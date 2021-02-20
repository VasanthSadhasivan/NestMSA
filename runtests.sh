#!/bin/bash

BOLD=$(tput bold)
NORMAL=$(tput sgr0)

g++ nest_msa.cpp -c

for filename in ./tests/*; do
    g++ -std=c++11 -Wall -o test tests-main.o nest_msa.o "$filename"

    echo "${BOLD}=================================================================================================${NORMAL}"
    echo "${BOLD}Testing $filename${NORMAL}"
    echo "${BOLD}-------------------------------------------------------------------------------------------------${NORMAL}"
    ./test
    echo "${BOLD}End of $filename${NORMAL}"
    echo "${BOLD}=================================================================================================${NORMAL}"
    echo
    echo

    rm test
done

rm nest_msa.o
