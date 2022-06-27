#!/bin/bash

codebooks=`ls | grep Codebook`
lambdas=('10') # '100' '250' '500' '1000' '5000' '10000')

for l in "${lambdas[@]}"; do
	./eval_q -l $l $codebooks test_im/*.pgm # 2> eval_l$l.txt
done
