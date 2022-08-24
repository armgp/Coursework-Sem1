#!/bin/bash
echo -n "Enter the Year: "
read YYYY

#get year less than or equal to 9999
if [ $YYYY -gt 9999 ]
then
	while [ $YYYY -gt 9999 ]
	do
		echo -n "Enter an Year less than 10000: "
		read YYYY
	done
fi

#convert year to 4 digits
if [ $YYYY -lt 10 ]
then
	YYYY="000$YYYY"
elif [ $YYYY -lt 100 ]
then
	YYYY="00$YYYY"
elif [ $YYYY -lt 1000  ]
then
	YYYY="0$YYYY"
fi


#getting day and month for possible palindrome
DD=$(echo $YYYY | cut -c 3-4 | rev )
MM=$(echo $YYYY | cut -c 1-2 | rev )
PAL="$DD-$MM-$YYYY"
#checking if month value is correct
if [ $MM -lt 1 ] || [ $MM -gt 12 ]
then
	echo -e "\e[1;31mNo Palindrome days available in the given year"
else
	#When month value is correct
	#If the month have 31 days
	if [[ $MM == 01 || $MM == 03 || $MM == 05 || $MM == 07 || $MM == 08 || $MM == 10 || $MM == 12 ]]
	then
		if [ $DD -gt 31 ] || [ $DD -lt 1 ]
		then
			echo -e "\e[1;31mNo Palindrome days available in the given year"
		else
			echo -e "\e[1;32m$PAL"
		fi
	
	#If the month is February
	elif [ $MM -eq 02 ]
	then
		#test for leap year
		if (( ($(($YYYY%4)) == 0 && $(($YYYY%100)) != 0) || $(($YYYY%400)) == 0  ))
		then
			#its a leap year
			if [ $DD -gt 29 ] || [ $DD -lt 1 ]
                	then
                        	echo -e "\e[1;31mNo Palindrome days available in the given year"
                	else
                        	echo -e "\e[1;32m$PAL"
                	fi
		else
			#its not a leap year
                        if [ $DD -gt 28 ] || [ $DD -lt 1 ]
                        then
                                echo -e "\e[1;31mNo Palindrome days available in the given year"
                        else
                                echo -e "\e[1;32m$PAL"
                        fi
		fi
	#If the month has 30 days
	else
		if [ $DD -gt 30 ] || [ $DD -lt 1 ]
                then
                        echo -e "\e[1;31mNo Palindrome days available in the given year"
                else
                        echo -e "\e[1;32m$PAL"
                fi

	fi	
fi

