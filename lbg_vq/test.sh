#!/bin/bash

while (($#)); do
	if [[ -f $1 ]]; then
		./test $1 test_im/*.pgm
	else
		echo "$1 does not exist"
	fi
	shift
done
