#!/bin/bash

codebooks=`ls | grep Codebook`
lambdas=('1' '50' '100' '500' '1000' '5000' '10000')

for l in "${lambdas[@]}"; do
	echo $l
	./encode -l $l $codebooks test_im/*.pgm # 2> eval_l$l.txt
done

./decode $codebooks test_im/*.dat
