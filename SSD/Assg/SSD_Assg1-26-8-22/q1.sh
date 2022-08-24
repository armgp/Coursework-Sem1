#!/bin/bash
#Assignment 1
echo -n  "Enter a number greater than 0:"
read n

while [ $n -lt 0 ]
do
	echo -n "Please Enter a number greater than 0"
	read n
done

echo

if [ $n -gt 80000 ]
then
	echo -e "\e[1;33m Kindly wait for a few seconds"
	echo
fi
 
echo -e "\e[1;35m <----------------Twin Primes || Product of Twin Primes || Digital Roots---------------->"
echo
print_twin_primes(){
	seive=(0)	
	for (( i=1; i<=$n; i++ ))
	do
		seive[$i]=0
	done

	for ((i=2; i<$n; i++))
	do
		if [ ${seive[$i]} -eq 0 ]
		then
			for((j=$i, indx=$i*$i; $indx<=$n; j++, indx=$i*$j))
			do
				if [ ${seive[$indx]} -eq 0 ]
				then
					seive[$indx]=1
				fi
			done
		fi
	done
	
	totSum=0
	for ((i=3, j=0; i<$n-1; i++))
	do	
		if [ ${seive[$i]} -eq 0 ] && [ ${seive[$(($i+2))]} -eq 0 ]
		then
			j=$(($j+1))
			product=$(($i*$(($i+2))))
			currProduct=$product
			sum=0; digRoot=1000

			while [ $digRoot -gt 10 ]
			do
				while [ $currProduct -gt 0 ]
				do
					sum=$(($sum+$(($currProduct%10))))
					currProduct=$(($currProduct/10))
				done	
				digRoot=$sum
				currProduct=$sum
				sum=0
			done

			echo -e "\e[1;36m                |$j."
			echo "                |Prime1: $i, Prime2: $(($i+2))"
			echo "                |Prime1*Prime2 = $product"
			echo "                |Digital Root = $digRoot"
			echo -e "\e[1;37m                |----------------------------------"

			totSum=$(($totSum+$digRoot))
		fi
	done

	echo
        echo "-------------------------------------------"
	echo -e "\e[1;32m   The Sum of all the Digital Roots = $totSum"
	echo -e "\e[1;37m -------------------------------------------"
}

print_twin_primes

