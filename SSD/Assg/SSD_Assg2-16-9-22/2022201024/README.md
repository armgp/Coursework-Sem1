#Software System Development - Assignment 2 – SQL

SSD 2nd Assignment

## Q1
Questions a, b and c share the same table and all the three subquestions are written
in the same file q1.sql. Each subsection answer code is separated by comments for ease of reading.
Code for importing the person table from csv file is also written in this file. The same table is reused in Q3.

###Q1(a)
Procedure newHikeSalary() is created which populates the table hike2022 with columns
{HikePK, EmpIDFK, FirstName, LastName, Gender, WeightInKg, LastHike, LastSalary, NewHike, NewSalary}.

###Q1(b)
Procedure getJoiningData() is created which populates the table PersonJoining with columns
{PJoinPK, EmpIDFK, FirstName, LastName, DateofBirth, Age, DateofJoining, DayofJoining, MonthofJoining,
YearofJoining, WorkExpinDays}

###Q1(c)
Procedure getTransferData() is created which populates the table PersonTransfer with columns
{PTPK, EmpIDFK, FirstName, LastName, Gender, DateofJoining, CurrentRegion, NewRegion}

## Q2
Data is imported from timezone.csv and country.csv into tables called “time_zone” (zonename,
countrycode, timezonecode, timestart, gmtoffset, dst) and “country” (countrycode, countryname) in my
local mysql database.

A function named timezoneconvert is created. It takes in a datetime and source and target timezone code.
Now the source datetime is converted to unix timestamp and all the gmtoffsets which has timestart less than
the source datetime timestamp is taken, out of which the closest value to the source datetime is found by arranging in 
descending order and then taking the topmost value. This offset is substarcted from the given source datetime to get 
the gmt time. Again the same process is repeated on the target timezone code and the resulting offset is added to the 
gmt datetime we got earlier to print the result.
Thus we are taking the offsets of the closest timestart to the given datetime only and printing the converted
value for that offsets only. No multiple values are printed.

Note: Here the function takes in put time in the format 'YYYY-MM-DD HH:MM:SS'

## Q3
We are using the same persons table imported in Q1 here. This returns a table with the following columns
{EmployeeRegion, No. Of Employee born between 00:00 hours to 08:00 hours, No. Of Employee born between
 08.01 hours to 15.00 hours, No. Of Employees born after 15:01 hours until 22:59 hours}
