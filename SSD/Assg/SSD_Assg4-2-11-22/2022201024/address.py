import csv
import os
import time

from prettytable import PrettyTable

slNo = 1
directory = {}

with open('dir_data.csv', 'r') as csvFile:
	csvReader = csv.reader(csvFile)
	next(csvReader)
	for line in csvReader:
		slNo = int(line[0])
		directory[slNo] = line[1:]
csvFile.close()
slNo+=1

fieldNames = ['SlNo', 'First Name', 'Last Name', 'Address', 'City', 'State', 'Zip', 'Contact number', 'Email address']

def printTable():
	os.system('clear')
	table = PrettyTable(fieldNames)
	for i in directory.keys():
		table.add_row([i]+directory[i])
	print(table)

def printOps():
	print('-----------------ADDRESS DIRECTORY-----------------')
	print('1. ADD ENTRY')
	print('2. DISPLAY DIRECTORY')
	print('3. REMOVE/UPDATE ENTRY')
	print('4. SEARCH')
	print('5. QUIT')
	
def printInvalid():
	print('INVALID OPTION')
	time.sleep(0.5)
	

while True:
	printOps()
	option = int(input('CHOOSE OPERATION - TYPE[1/2/3/4/5] AND ENTER: '))
	if option==1:
		os.system('clear')
		firstName = input('FIRST NAME: ')
		lastName = input('LAST NAME: ')
		address = input('ADDRESS: ')
		city = input('CITY: ')
		state = input('STATE: ')
		zipcode = input('ZIP: ')
		contactNo = input('CONTACT NUMBER: ')
		email = input('EMAIL ADDRESS: ')
		details = [firstName, lastName, address, city, state, zipcode, contactNo, email]
		directory[slNo] = details
		slNo+=1
		print('\n ******ENTRY SUCCESSFULLY ADDED****** \n')
		
	elif option==2:
		printTable()
	elif option==3:
		printTable()
		print('1. REMOVE', '2. UPDATE')
		opt = int(input('CHOOSE OPTION: '))
		
		if opt==1:
			row = input('ENTER SlNo OF ROW TO BE REMOVED: ')
			index = int(row)
			if index in directory.keys():
				del directory[index]
				print(directory)
			else:
				printInvalid()
		elif opt==2:
			rowInd = int(input('ENTER SlNo OF ROW TO BE UPDATED: '))
			if rowInd in directory.keys():
				table = PrettyTable([1,2,3,4,5,6,7,8])
				table.add_row(directory[rowInd])
				print(table)
				colInd = int(input('ENTER COLUMN NO: OF VALUE TO BE UPDATED: '))
				if colInd in [1,2,3,4,5,6,7,8]:
					colInd-=1
					value = input('ENTER NEW VALUE: ')
					directory[rowInd][colInd] = value
				else:
					printInvalid()
			else:
				printInvalid()
			
		else:
			printInvalid()
		
		printTable()
	elif option==4:
		os.system('clear')
		table = PrettyTable([0,1,2,3,4,5,6,7,8])
		table.add_row(fieldNames)
		print(table)
		key = int(input('CHOOSE SEARCH KEY NUMBER FROM THE ABOVE OPTIONS: '))
		val = input('ENTER THE VALUE TO SEARCH FOR: ')
		if key==0:
			k = int(val)
			if k in directory.keys():
				print('------ENTRY FOUND------')
				foundTable = PrettyTable(directory[k])
				print(foundTable)
			else:
				print('------ENTRY DOESN\'T EXIST------')
		elif key in [1,2,3,4,5,6,7,8]:
			index = key-1
			res = []
			for i in directory.keys():
				if directory[i][index]==val:
					a = [i]
					a = a+directory[i]
					res.append(a)
			n = len(res)
			if n>0:
				print('------ENTRY FOUND------')
				foundTable = PrettyTable(fieldNames)
				for arr in res:
					foundTable.add_row(arr)
				print(foundTable)
			else:
				print('------ENTRY DOESN\'T EXIST------')
		else:
			printInvalid()
				
	elif option==5:
		os.system('clear')
		f = open('dir_data.csv', "w+")
		csvWriter = csv.writer(f)
		csvWriter.writerow(fieldNames)
		for i in directory.keys():
			line = [i]
			line = line + directory[i]
			csvWriter.writerow(line)
		f.close()
		break
	else:
		printInvalid()
