#!/bin/bash

gcc -o test max.c

grep -E '^-[;][^;]+;-[;][0-9]+;-$' c-wildwater_v3.dat | ./test

for f in top10 bottom50; do
    gnuplot -e "set datafile separator ','; set terminal pngcairo; set output '${f}.png'; plot '${f}.csv' using 2:xtic(1) with boxes"
done
