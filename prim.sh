#!/bin/bash
rm -fr a.out
nvcc -O3 -pg -std=c++11 *.cpp *.cu --gpu-architecture=compute_35 --gpu-code=sm_35 -I ./include -DIFHOP=1 -DSERT=50 -DNODE=200 -DSERT=100
./a.out
#gprof -b a.out gmon.out >report.txt
