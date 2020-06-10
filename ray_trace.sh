#!/bin/bash
bin='bin_ray'
echo "::Compiling ---"
g++ *.cpp -o $bin -lpthread

if [ $? -eq 0 ]
then
	echo "::Running ---"
	./$bin
	eog out.ppm
	fi
