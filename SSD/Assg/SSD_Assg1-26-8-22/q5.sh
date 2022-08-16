#!/bin/bash
echo -n "Enter a value for n: "
read ln
l0=2
l1=1

if [ $ln -eq 0 ] 
then 
	echo $l0
elif [ $ln -eq 1 ]
then
	echo $l1
else
	for (( i=0; i<ln; i++ ))
	do
		temp=$l0
		l0=$l1
		l1=$(bc <<< $temp+$l1)
	done
	echo $l0
fi
