#!/bin/bash
set -e
GOVERNOR=performance
SYS_PATH=/sys/devices/system/cpu
for cpu in `ls $SYS_PATH | grep 'cpu[0-9]\+'`; do
        CPU_SCALING_GOVERNOR=$SYS_PATH/$cpu/cpufreq/scaling_governor
        current=`cat $CPU_SCALING_GOVERNOR`
        if [ "$current" != "$GOVERNOR" ]; then
                echo "$cpu is currently '$current', setting to '$GOVERNOR'"
                echo -n $GOVERNOR | sudo tee -a $CPU_SCALING_GOVERNOR > /dev/null
        fi
done
git clean -dfx
if [ ! -f runall ]; then
    g++ -O3 runall.cpp -o runall
fi
TS=""
if [ "$HOSTNAME" = "opi5" ]; then
        TS="taskset -c 4-7"
fi
if [ "$HOSTNAME" = "rock5b" ]; then
        TS="taskset -c 4-7"
fi
$TS ./runall $HOSTNAME $1
