#!/bin/bash

KAPREKARS_CONST=6174

echo -n "Enter a 4 digit number: "
read number

while [ $number -eq 1111 ] || [ $number -eq 0000 ] || [ ${#number} -ne 4  ]
do
	if [[ $number==1111 || $number==0000  ]]
	then
		while (( $number==1111 || $number==0000 )) 
		do
			echo -n "Please enter a number other than 1111 or 0000: "
			read number
		done
	fi

	if [[ ${#number} != 4 ]]
	then
		while [ ${#number} -ne 4 ] 
		do
			echo -n "Please enter a 4 digit number: "
			read number
		done
	fi
done

while [ $number -ne $KAPREKARS_CONST ]
do
	echo -en "\e[1;31m $number, "
	asc_num=$(echo $number | grep -o . | sort | tr -d "\n")
	desc_num=$(echo $number | grep -o . | sort -r | tr -d "\n")
	number=$(expr $desc_num - $asc_num)

	if [[ ${#number} != 4 ]]
	then
		number="0$number"
	fi

done
echo -en "\e[1;32m $number"
echo




