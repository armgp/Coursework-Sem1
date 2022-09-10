DROP TABLE person;
CREATE TABLE IF NOT EXISTS person (
    EMPID INT NOT NULL,
    Name_Prefix VARCHAR(255) NOT NULL,
    First_Name VARCHAR(255) NOT NULL,
    Middle_Initial VARCHAR(2) NOT NULL,
    Last_Name VARCHAR(255) NOT NULL,
    Gender VARCHAR(2) NOT NULL,
    Email VARCHAR(255) NOT NULL,
    Fathers_Name VARCHAR(255) NOT NULL,
    Mothers_Name VARCHAR(255) NOT NULL,
    Mothers_Maiden_Name VARCHAR(255) NOT NULL,
    Date_Of_Birth VARCHAR(255) NOT NULL,
    Time_Of_Birth TIME NOT NULL,
    Weight_in_Kgs INT NOT NULL,
    Date_Of_Joining VARCHAR(255) NOT NULL,
    Salary INT NOT NULL,
    Last_Percent_Hike VARCHAR(20) NOT NULL,
    Place_Name VARCHAR(255) NOT NULL,
    County VARCHAR(255) NOT NULL,
    City VARCHAR(255) NOT NULL,
    State VARCHAR(10) NOT NULL,
    Region VARCHAR(255) NOT NULL,
    PRIMARY KEY (EMPID)
);

SHOW VARIABLES LIKE "secure_file_priv"; 

LOAD DATA INFILE '/var/lib/mysql-files/empdetails.csv' 
INTO TABLE person 
FIELDS TERMINATED BY ',' 
ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 ROWS
(EMPID,Name_Prefix,First_Name,Middle_Initial,Last_Name,Gender,Email,Fathers_Name,Mothers_Name,Mothers_Maiden_Name,@Date_Of_Birth,@Time_Of_Birth,Weight_in_Kgs,@Date_Of_Joining,Salary,Last_Percent_Hike,Place_Name,County,City,State,Region)
SET Date_Of_Birth = STR_TO_DATE(@Date_Of_Birth, '%m/%d/%Y'), Time_Of_Birth = STR_TO_DATE(@Time_Of_Birth, '%h:%i:%s %p'), Date_Of_Joining = STR_TO_DATE(@Date_Of_Joining, '%m/%d/%Y');

select * from person;

 

-- ----------------------------------------------------#Q1-(a)----------------------------------------------------------------------------------------------------
DROP PROCEDURE newHikeSalary;
DROP TABLE hike2022;
DELIMITER $$
CREATE PROCEDURE newHikeSalary()
BEGIN
	DECLARE finished INTEGER DEFAULT 0;
    DECLARE empiddetail varchar(100) DEFAULT "";
    DECLARE fndetail varchar(100) DEFAULT "";
    DECLARE lndetail varchar(100) DEFAULT "";
	DECLARE gdetail varchar(100) DEFAULT "";
	DECLARE wdetail varchar(100) DEFAULT "";
    DECLARE lhdetail varchar(100) DEFAULT "";
    DECLARE lsdetail varchar(100) DEFAULT "";
    
	-- declare cursor 
	DEClARE details 
		CURSOR FOR 
			SELECT EMPID, First_Name, Last_Name, Gender, Weight_in_Kgs, Last_Percent_Hike, Salary FROM person WHERE Weight_in_Kgs<50;

	-- declare NOT FOUND handler
	DECLARE CONTINUE HANDLER 
        FOR NOT FOUND SET finished = 1;
	
    SET SQL_SAFE_UPDATES = 0;
	CREATE TABLE IF NOT EXISTS hike2022 (
		HikePK INT NOT NULL AUTO_INCREMENT,
		EmpIDFK INT,
		FirstName varchar(255),
		LastName varchar(255),
		Gender varchar(255),
		WeightInKg INT,
		LastHike varchar(255),
		LastSalary INT,
		NewHike varchar(255),
		NewSalary FLOAT,
        PRIMARY KEY (HikePK),
		FOREIGN KEY (EmpIDFK) REFERENCES person(EMPID)
	);
    DELETE FROM hike2022;

	OPEN details;

	getDetail: LOOP
		FETCH details INTO empiddetail, fndetail, lndetail, gdetail, wdetail, lhdetail, lsdetail;
		IF finished = 1 THEN 
			LEAVE getDetail;
		END IF;
        
        SET @nh = CAST(lhdetail AS UNSIGNED) + 2;
        SET @nh = CONCAT(@nh, '%');
        SET @ns = (100+@nh)*CAST(lsdetail AS FLOAT);
        SET @ns = @ns/100;
        INSERT INTO hike2022 (EmpIDFK, FirstName, LastName, Gender, WeightInKg, LastHike, LastSalary, NewHike, NewSalary)
		VALUES (empiddetail, fndetail, lndetail, gdetail, wdetail, lhdetail, lsdetail, @nh, @ns);
	END LOOP getDetail;
    
	CLOSE details;
END$$
DELIMITER ;

CALL newHikeSalary(); 
select * from hike2022;

-- ----------------------------------------------------#Q1-(b)----------------------------------------------------------------------------------------------------
DROP PROCEDURE getJoiningData;
DROP TABLE PersonJoining;
DELIMITER $$
CREATE PROCEDURE getJoiningData()
BEGIN
	DECLARE finished INTEGER DEFAULT 0;
    DECLARE empiddetail varchar(100) DEFAULT "";
    DECLARE fndetail varchar(100) DEFAULT "";
    DECLARE lndetail varchar(100) DEFAULT "";
	DECLARE dobdetail varchar(100) DEFAULT "";
	DECLARE dojdetail varchar(100) DEFAULT "";
    
	-- declare cursor 
	DEClARE details 
		CURSOR FOR 
			SELECT EMPID, First_Name, Last_Name, Date_Of_Birth, Date_Of_Joining FROM person;

	-- declare NOT FOUND handler
	DECLARE CONTINUE HANDLER 
        FOR NOT FOUND SET finished = 1;
	
    SET SQL_SAFE_UPDATES = 0;
    #{PJoinPK, EmpIDFK, FirstName, LastName, DateofBirth, Age, DateofJoining, DayofJoining, MonthofJoining, YearofJoining, WorkExpinDays} 
	CREATE TABLE IF NOT EXISTS PersonJoining (
		PJoinPK INT NOT NULL AUTO_INCREMENT,
		EmpIDFK INT,
		FirstName varchar(255),
		LastName varchar(255),
        DateofBirth DATE,
        Age INT,
        DateofJoining DATE,
        DayofJoining INT,
        MonthofJoining VARCHAR(20),
        YearofJoining INT,
        WorkExpinDays INT,
        PRIMARY KEY (PJoinPK),
		FOREIGN KEY (EmpIDFK) REFERENCES person(EMPID)
	);
    DELETE FROM PersonJoining;

	OPEN details;

	getDetail: LOOP
		FETCH details INTO empiddetail, fndetail, lndetail, dobdetail, dojdetail;
		IF finished = 1 THEN 
			LEAVE getDetail;
		END IF;
        
        SET @age = timestampdiff(YEAR, dobdetail, current_date());
        SET @dayoj = EXTRACT(DAY FROM dojdetail);
        SET @monthoj = MONTHNAME(dojdetail);
        SET @yearoj = EXTRACT(YEAR FROM dojdetail);
        SET @wexp = datediff(current_date(), dojdetail);
        INSERT INTO PersonJoining (EmpIDFK, FirstName, LastName, DateofBirth, Age, DateofJoining, DayofJoining, MonthofJoining, YearofJoining, WorkExpinDays)
		VALUES (empiddetail, fndetail, lndetail, dobdetail, @age, dojdetail, @dayoj, @monthoj, @yearoj, @wexp);
	END LOOP getDetail;
    
	CLOSE details;
END$$
DELIMITER ;

CALL getJoiningData(); 
select * from PersonJoining;

-- ----------------------------------------------------#Q1-(c)----------------------------------------------------------------------------------------------------
DROP PROCEDURE getTransferData;
DROP TABLE PersonTransfer;
DELIMITER $$
CREATE PROCEDURE getTransferData()
BEGIN
	DECLARE finished INTEGER DEFAULT 0;
    DECLARE empiddetail varchar(100) DEFAULT "";
    DECLARE fndetail varchar(100) DEFAULT "";
    DECLARE lndetail varchar(100) DEFAULT "";
	DECLARE gdetail varchar(100) DEFAULT "";
	DECLARE dojdetail varchar(100) DEFAULT "";
    DECLARE crdetail varchar(100) DEFAULT "";
    
	-- declare cursor 
	DEClARE details 
		CURSOR FOR 
			SELECT EMPID, First_Name, Last_Name, Gender, Date_Of_Joining, Region FROM person;

	-- declare NOT FOUND handler
	DECLARE CONTINUE HANDLER 
        FOR NOT FOUND SET finished = 1;
	
    SET SQL_SAFE_UPDATES = 0;
    #{PTPK, EmpIDFK, FirstName, LastName, Gender, DateofJoining, CurrentRegion, NewRegion} 
	CREATE TABLE IF NOT EXISTS PersonTransfer (
		PTPK INT NOT NULL AUTO_INCREMENT,
		EmpIDFK INT,
		FirstName varchar(255),
		LastName varchar(255),
        Gender varchar(2),
        DateofJoining DATE,
        CurrentRegion varchar(255),
        NewRegion varchar(255),
        PRIMARY KEY (PTPK),
		FOREIGN KEY (EmpIDFK) REFERENCES person(EMPID)
	);
    DELETE FROM PersonTransfer;

	OPEN details;

	getDetail: LOOP
		FETCH details INTO empiddetail, fndetail, lndetail, gdetail, dojdetail, crdetail;
		IF finished = 1 THEN 
			LEAVE getDetail;
		END IF;
        
        SET @workExp = timestampdiff(YEAR, dojdetail, current_date());
        
        IF gdetail='F' AND @workExp>10 THEN
			SET @nr = 'DC';
		ELSEIF gdetail='M' AND @workExp>20 THEN
			SET @nr = 'Capitol';
		ELSE
			SET @nr = crdetail;
		END IF;
        
        INSERT INTO PersonTransfer (EmpIDFK, FirstName, LastName, Gender, DateofJoining, CurrentRegion, NewRegion)
		VALUES (empiddetail, fndetail, lndetail, gdetail, dojdetail, crdetail, @nr);
	END LOOP getDetail;
    
	CLOSE details;
END$$
DELIMITER ;

CALL getTransferData(); 
select * from PersonTransfer ORDER BY FirstName ASC;