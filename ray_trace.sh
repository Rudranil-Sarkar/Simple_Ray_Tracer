#!/bin/bash
bin='bin_ray'
g++ *.cpp -o $bin

if [ $? -eq 0 ]
then
	./$bin
	eog out.ppm
	fi
