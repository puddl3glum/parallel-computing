#! /bin/bash

total=0

runs=3

for i in `seq 1 $runs`;
do
        time_taken=$( { /usr/bin/time -f "%e" ./bin/conway $1 $1 $2; } 2>&1 )
        echo $time_taken
        total=$(bc -l <<< "$total + $time_taken")
done

avg=$(bc -l <<< "$total / 3")
echo $avg
