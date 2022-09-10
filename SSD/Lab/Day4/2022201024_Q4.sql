CREATE TABLE Persons (
    cname varchar(255),
    city varchar(255),
    country varchar(255),
    grade varchar(255)
);

DELIMITER $$
CREATE PROCEDURE createDetailsList (
	INOUT allDetails varchar(4000), INOUT custCity varchar(4000), INOUT custCountry varchar(4000), INOUT custGrade varchar(4000)
)
BEGIN
	DECLARE finished INTEGER DEFAULT 0;
	DECLARE ndetail varchar(100) DEFAULT "";
    DECLARE cdetail varchar(100) DEFAULT "";
    DECLARE codetail varchar(100) DEFAULT "";
    DECLARE gdetail varchar(100) DEFAULT "";
    
	-- declare cursor for employee email
	DEClARE nameDetail 
		CURSOR FOR 
			SELECT CUST_NAME, CUST_CITY, CUST_COUNTRY, GRADE FROM customer WHERE AGENT_CODE LIKE "A00%";

	-- declare NOT FOUND handler
	DECLARE CONTINUE HANDLER 
        FOR NOT FOUND SET finished = 1;

	OPEN nameDetail;

	getDetail: LOOP
		FETCH nameDetail INTO ndetail, cdetail, codetail, gdetail;
		IF finished = 1 THEN 
			LEAVE getDetail;
		END IF;
		-- build email list
        INSERT INTO Persons (cname, city, country, grade)
		VALUES (ndetail, cdetail, codetail, gdetail);
        
    
	END LOOP getDetail;
    SELECT REPLACE(allDetails," ","");
	CLOSE nameDetail;

END$$
DELIMITER ;

CALL createDetailsList(@allDetails, @custCity, @custCountry, @custGrade); 
select * from Persons;
