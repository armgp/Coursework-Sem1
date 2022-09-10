#!/bin/bash

n=$(wc -l $1 | awk '{print $1}')

if (( $(($n%2)) == 1 ))
then
	N=$(($(($n/2))+1))
else
	N=$(($n/2))
fi

head -$N $1 | tail -1 
