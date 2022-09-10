DELIMITER &&  
CREATE PROCEDURE addTwoNumbers(IN num1 INT, IN num2 INT, OUT sum INT)  
BEGIN  
    SELECT num1 + num2 INTO sum;   
END &&  
DELIMITER ;  

CALL addTwoNumbers(2, 3, @sum);
SELECT @sum;
