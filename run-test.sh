#! /bin/bash

total=0

runs=3

width=10
height=10
generations=10
chance=0.3

./bin/conway -w$width -h$height -g$generations -c$chance -s1
# ./saxpy

