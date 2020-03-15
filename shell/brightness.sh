#!/bin/bash

VAL=$(cat /sys/class/backlight/intel_backlight/brightness)
VAL_MAX=$(cat /sys/class/backlight/intel_backlight/max_brightness)
PERC=$((VAL*100/VAL_MAX))

if [ $1 == "-inc" ]
then
       PERC=$(($PERC+$2))
fi

if [ $1 == "-dec" ]
then
       PERC=$(($PERC-$2))
fi

if [ $PERC -lt 5 ]
then
    PERC=5
fi

if [ $PERC -gt 100 ]
then
    PERC=100
fi

VAL=$(($VAL_MAX*$PERC/100))
echo $VAL>/sys/class/backlight/intel_backlight/brightness
