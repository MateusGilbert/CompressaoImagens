#!/bin/bash

files=`grep -L -E '^(0,)+0$' B[1-9]_Codebooks/*`

x=0;y=0
re='^[1-9][0-9]?$'
for file in $files; do
	for aux in $(echo $file | tr '_' '\n'); do
		if [[ $aux =~ $re ]]; then
			x=$y; y=$aux;
		fi
	done

	string='0'
	for _ in $(seq $((x*y - 1))); do
		string+=`echo ',0'`
	done
	echo $string >> $file
done
