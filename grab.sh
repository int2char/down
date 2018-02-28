#!/bin/bash
rm -fr a.out
sers=(2 5 10 20 30 50 80 100)
for s in ${sers[@]}
do
	nvcc -O3 -pg -std=c++11 *.cpp *.cu --gpu-architecture=compute_35 --gpu-code=sm_35 -I ./include -DSERT=$s -DIFHOP=0 
	./a.out F 1>>FDATA.txt
	./a.out P 1>>PDATA.txt
	./a.out S 1>>SDATA.txt
	rm -fr a.out
	nvcc -O3 -pg -std=c++11 *.cpp *.cu --gpu-architecture=compute_35 --gpu-code=sm_35 -I ./include -DSERT=$s -DIFHOP=1
	./a.out F 1>>FDATA.txt
	./a.out P 1>>PDATA.txt
	./a.out S 1>>SDATA.txt
done
#gprof -b a.out gmon.out >report.txt
