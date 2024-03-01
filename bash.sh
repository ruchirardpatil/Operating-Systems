: '
	Oishik Mandal
	PG 41	G3

	Assignment 1
	1. Addition, Substraction, Multiplication, Division via control structures.
	2. Case conversion on strings
'

#!\bin\bash
#usage :bash PG41_ass1_bash.sh
function add() {		# to perform addition

	sum=`expr $num1 + $num2`
  	echo "Addition of $num1 and $num2 is $sum" 
}

function sub() { 		# to perform substraction
 	sub=`expr $num2 - $num1`
 	echo "Subtraction of $num2 and $num1 is $sub"
}

function mul() { 		# to perform multiplication
  	mul=`expr $num1 \* $num2` 
  	echo "Multiplication of $num1 and $num2 is $mul"
}

function div() { 		# to perform division
  	div=`expr "scale=5; $num1 / $num2"|bc`
  	echo "Division of $num1 and $num2 is $div"
}

function mod() {		# to find modulus
  	mod=`expr $num1 % $num2`
  	echo "Modulus of $num1 and $num2 is $mod"
}

function toUpper() { 		# to get uppercase
 	name=${name^^}
  	echo "converted to upper case: "
  	echo "$name"
}

function toLower() {		# to get lowercase
  	name=${name,,}
  	echo "converted to lower case: "
  	echo "$name"
}

echo " ------------- Program for arithmatic operations and string operations ------------- "
while [ 1 ]
do
	echo "Enter Your Choice"
	echo -e "1. Addition \n2. Subtration \n3. Multiplication \n4. Division\n5. Modulus\n6. Lowercase to Uppercase\n7. Uppercase to Lowercase\n8. Exit"
	read op

	if [ $op -eq 1 -o $op -eq 2 -o $op -eq 3 -o $op -eq 4 -o $op -eq 5 ] ; then # we need to read two numbers then
		echo -n "enter the number1: "           
		read num1                            
		echo -n "enter the number2: "
		read num2
	elif [ $op -eq 6 -o $op -eq 7 ] ; then # we need to read a string
		echo -n "enter the string: " 
		read name
	fi

	case $op in
		1) add;;
		2) sub;;
		3) mul;;
		4) div;;
		5) mod;;
		6) toUpper;;
		7) toLower;;
		8) echo "Goodbye !!!"
			exit;;
		*) echo "Invalid choice, try again";;
	esac
done

: '	OUTPUT

oishik@DESKTOP-MA69803:/mnt/c/Users/oishi/Documents/practical/OS$ bash PG41_ass1_bash.sh
 ------------- Program for arithmatic operations and string operations -------------
Enter Your Choice
1. Addition
2. Subtration
3. Multiplication
4. Division
5. Modulus
6. Lowercase to Uppercase
7. Uppercase to Lowercase
8. Exit
1
enter the number1:15
enter the number2:20
Addition of 15 and 20 is 35
Enter Your Choice
1. Addition
2. Subtration
3. Multiplication
4. Division
5. Modulus
6. Lowercase to Uppercase
7. Uppercase to Lowercase
8. Exit
2
enter the number1:2
enter the number2:20
Subtraction of 20 and 2 is 18
Enter Your Choice
1. Addition
2. Subtration
3. Multiplication
4. Division
5. Modulus
6. Lowercase to Uppercase
7. Uppercase to Lowercase
8. Exit
3
enter the number1:10
enter the number2:2
Multiplication of 10 and 2 is 20
Enter Your Choice
1. Addition
2. Subtration
3. Multiplication
4. Division
5. Modulus
6. Lowercase to Uppercase
7. Uppercase to Lowercase
8. Exit
4
enter the number1:15
enter the number2:3
Division of 15 and 3 is 5.00000
Enter Your Choice
1. Addition
2. Subtration
3. Multiplication
4. Division
5. Modulus
6. Lowercase to Uppercase
7. Uppercase to Lowercase
8. Exit
5
enter the number1:6
enter the number2:4
Modulus of 6 and 4 is 2
Enter Your Choice
1. Addition
2. Subtration
3. Multiplication
4. Division
5. Modulus
6. Lowercase to Uppercase
7. Uppercase to Lowercase
8. Exit
6
enter the lower case string:monday
converted to upper case:
MONDAY
Enter Your Choice
1. Addition
2. Subtration
3. Multiplication
4. Division
5. Modulus
6. Lowercase to Uppercase
7. Uppercase to Lowercase
8. Exit
7
enter the lower case string:MONDAY
converted to lower case:
monday
Enter Your Choice
1. Addition
2. Subtration
3. Multiplication
4. Division
5. Modulus
6. Lowercase to Uppercase
7. Uppercase to Lowercase
8. Exit
8
Goodbye !!!

'