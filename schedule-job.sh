#! /bin/bash



sbatch --qos=class -J 5000-5000 --begin=2019-02-04T15:49:16 --requeue --mail-user=wjsetzer@uab.edu -o 5000-5000.o%A -t 0:40:0 -N 1-1 -n 1 --mem-per-cpu=1000mb --constraint=dmc
