#!/bin/bash
if [ ! -f genreadme ]; then
    g++ -O3 -std=c++20 genreadme.cpp -o genreadme
fi
scp pi@pi4b:AoC-2023-cplusplus/pi4b .
scp pi@rock5b:AoC-2023-cplusplus/rock5b .
scp pi@pi5:AoC-2023-cplusplus/pi5 .
./genreadme
