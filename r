#!/bin/bash
perf
make -j$(nproc)
TS=
if [ $HOSTNAME = "rock5b" ] || [ $HOSTNAME = "opi5" ]; then
    TS=taskset -c 4-7
fi
$TS ./runall $HOSTNAME
