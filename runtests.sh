#!/bin/bash

BOLD=$(tput bold)
NORMAL=$(tput sgr0)

g++ nest_msa.cpp -c
g++ -std=c++11 -Wall -o tests tests-main.o nest_msa.o tests-tests.cpp

echo "${BOLD}=================================================================================================${NORMAL}"
echo "${BOLD}Testing create_peer_matrix()${NORMAL}"
echo "${BOLD}-------------------------------------------------------------------------------------------------${NORMAL}"
./tests [create_peer_matrix]
echo "${BOLD}=================================================================================================${NORMAL}"
echo

rm nest_msa.o
rm tests
