
# Assignment 4 – Python
## Software System Development – Monsoon 2022

### Q1 - pattern.py 
	1. Open Terminal an navigate to the folder containing file pattern.py
	2. Run command 'python3 pattern.py'

### Q2 - address.py 
	1. Install Dependency - pip install prettytable
	2. Open Terminal an navigate to the folder containing file address.py
	3. Run command 'python3 address.py'
	4. The screen shows the following
			1. ADD ENTRY
			2. DISPLAY DIRECTORY
			3. REMOVE/UPDATE ENTRY
			4. SEARCH
			5. QUIT
			CHOOSE OPERATION - TYPE[1/2/3/4/5] AND ENTER:
	   At the promt enter the option 
	   if you enter 1 -> prompt for filling each column value of the new entry is printed and you have to fill all of it and it will be added to the table
	   if you enter 2 -> directory will be printed as a table
	   if you enter 3 -> 
	   		2 more options are shown as below
			1. REMOVE 2. UPDATE
				CHOOSE OPTION: 
				On choosing 1 -> 
					ENTER SlNo OF ROW TO BE REMOVED: 
					prompt is shown where you enter the slNo of the row to be deleted
				On choosing 2 ->
					ENTER SlNo OF ROW TO BE UPDATED:
					prompt is shown where you enter the slNo of the row to be updated
					On entering this value you get another prompt
					ENTER COLUMN NO: OF VALUE TO BE UPDATED: 
					On entering this value you get another prompt
					ENTER NEW VALUE:
					Here you enter the updated value
		if you enter 4 -> 
			you get the below prompt
			+------+------------+-----------+---------+------+-------+-----+----------------+---------------+
			|  0   |     1      |     2     |    3    |  4   |   5   |  6  |       7        |       8       |
			+------+------------+-----------+---------+------+-------+-----+----------------+---------------+
			| SlNo | First Name | Last Name | Address | City | State | Zip | Contact number | Email address |
			+------+------------+-----------+---------+------+-------+-----+----------------+---------------+
			CHOOSE SEARCH KEY NUMBER FROM THE ABOVE OPTIONS: 
			On entering a value for this you get another prompt
			ENTER THE VALUE TO SEARCH FOR:
		if you enter 5 -> Exits from the program
	5. data is read from and written into a csv file named dir_data.csv
### Q3 - map.py
	1. Open Terminal an navigate to the folder containing file map.py
	2. Run command 'python3 map.py'
	3. command data is read from csv file named commands.csv - Command can be given in mm or cm.
	4. Here starting position S is taken as the origin = (0, 0)
