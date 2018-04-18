#!/bin/bash

rm -f *.out
nvcc -O3  -std=c++11 *.cpp *.cu --gpu-architecture=compute_35 --gpu-code=sm_35 -I ../include -I ../cplex_include -L ../lib -lconcert -lcplex -lilocplex -lm -lpthread -DIL_STD -DNODE=50 -DEDge=222 -DTask=$2500 -DTYPE=0 #2>>./data/complie.txt
# CUDA_VISIBLE_DEVICES=0 ./a.out G T 1>>./data/runinfo.txt 2>>./data/err.txt


