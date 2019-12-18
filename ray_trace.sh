#!/bin/bash
bin='bin_ray'
echo "::Compiling ---"
g++ *.cpp -o $bin

if [ $? -eq 0 ]
then
	echo "::Running ---"
	./$bin
	eog out.ppm
	fi
