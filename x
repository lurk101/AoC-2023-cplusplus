#!/bin/bash
for day in 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
do
    cp day01/day01.txt day$day/day$day.txt
    cp day01/day01.md day$day/day$day.md
    cp day01/day01.cpp day$day/day$day.cpp
    sed -i "s/Day 01/Day $day/" day$day/day$day.cpp
    sed -i "s/day01/day$day/" day$day/day$day.cpp
done
