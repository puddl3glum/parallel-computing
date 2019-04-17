#! /bin/bash

total=0

runs=3

width=100
height=100
generations=100

for i in `seq 1 $runs`;
do
        time_taken=$( { /usr/bin/time -f "%e" ./bin/conway -w$width -h$height -g$generations -s1; } 2>&1 )
        echo $time_taken
        total=$(bc -l <<< "$total + $time_taken")
done

avg=$(bc -l <<< "$total / 3")
echo $avg
