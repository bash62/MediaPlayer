#!/bin/bash

mkdir -p ./bin

cd ./progs/MonPG1
make
cd ../MonPG2
make
gcc -Wall ../MonPG3.c -o ../../bin/MonPG3
gcc -Wall ../MonPG4.c -o ../../bin/MonPG4
gcc -Wall ../MonPG5.c -o ../../bin/MonPG5