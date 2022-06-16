#!/bin/bash

declare -a N=('16' '64' '128' '256' '512' '1024')   # ('320' '640' '1280', '2560')
declare -a X=('1' '2' '4' '8')
declare -a Y=('1' '2' '4' '8')

for n in "${N[@]}"; do
	for x in "${X[@]}"; do
		for y in "${Y[@]}"; do
			./train -n $n -d $x $y -f train_im/*.pgm
		done
	done
done