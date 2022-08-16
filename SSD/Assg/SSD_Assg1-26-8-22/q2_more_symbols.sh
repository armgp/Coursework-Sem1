#!/bin/bash

echo -e "\e[1;33m ----------Password Generator----------"

RANGE=$((20-6+1))

#ensures password length is between 6 and 20
PASS_LEN=$(($(($RANDOM%$RANGE))+6))

#creates random string of length PASS_LEN
string=$(</dev/urandom tr -dc '_A-Z-a-z-0-9-!@#$%^&*()_+-}{><.,/"\:;~' | head -c${1:-$PASS_LEN})

stat="true"
while [ $stat == "true" ]
do
	#checks if the string has atleast one uppercase, lowercase, digits and special character, and the string starts with either lower/uppercase alphabet
	if [[ $string =~ .*[A-Z].* && $string =~ .*[a-z].* && $string =~ .*[0-9].* && $string =~ .*['!@#$%^&*()_+-}{><.,/\:;~"'].* && $string =~ ^[A-Za-z].* ]]
	then
		#check if the string doesnt have lowercase and number coming together in any order
		#check if the string doesnt start with symbol
		#check if the string doesnt have two symbols together
		#check if the password doesnt end with capital letters
		if [[ ! $string =~ .*[a-z][0-9].* && ! $string =~ .*[0-9][a-z].* && ! $string =~ ^['!@#$%^&*()_+-}{><.,/\:;~"'].* && ! $string =~ .*['!@#$%^&*()_+-}{><.,/\:;~"']['!@#$%^&*()_+-}{><.,/\:;~"'].*  && ! $string =~ .*[A-Z]$ ]]   
		then
			#since our string is tested through the conditions
		       	#string could only end with a lowercase letters/numbers/symbols and start with a upper/lowercase letter
			# and the string could never have repeating lowercase letters
			# and the minimum length of the string is 6, it could never be a palindrome
			#code for checking for palindrome is therefore not necessary

			#check if the string has repeating lowercase charachters	
			if ! echo $string | grep -q '\([a-z]\).*\1'
			then
				echo -e -n "\e[1;37m The password of length $PASS_LEN: "
                                echo -e "\e[1;31m $string"  	
				stat="false"
                                break
			fi
		fi
	fi


	PASS_LEN=$(($(($RANDOM%$RANGE))+6))
	string=$(</dev/urandom tr -dc '_A-Z-a-z-0-9-!@#$%^&*()_+-}{><.,/"\:;~' | head -c${1:-$PASS_LEN})
done
#done

