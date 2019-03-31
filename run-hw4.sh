#! /bin/bash

module load openmpi

total=0

runs=3

threads=32
width=5000
height=5000
gen=5000

for i in `seq 1 $runs`;
do
  time_taken=$( { /usr/bin/time -f "%e" mpirun -np $threads ./bin/conway $width $height $gen;  } 2>&1  )
  echo $time_taken
  total=$(bc -l <<< "$total + $time_taken")
done

avg=$(bc -l <<< "$total / 3")
echo $avg

