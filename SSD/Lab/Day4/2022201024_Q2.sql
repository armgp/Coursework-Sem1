DELIMITER &&  
CREATE PROCEDURE getCustomersWhoLiveInBanglore(IN city VARCHAR(20))  
BEGIN  
    SELECT CUST_NAME FROM customer WHERE WORKING_AREA=city;
END &&  
DELIMITER ;  

CALL getCustomersWhoLiveInBanglore("Bangalore");
