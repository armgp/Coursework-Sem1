SELECT Region , TOB1 'No. Of Employee born between 00:00 hours to 08:00 hours', 
TOB2 'No. Of Employee born between 08:01 hours to 15:00 hours', 
TOB3 'No. Of Employee born between 15:01 hours to 22:59 hours'
FROM
(SELECT Region, count(Time_Of_Birth) as TOB1
FROM person 
WHERE Time_Of_Birth BETWEEN '00:00:00' AND '08:00:00'
GROUP BY Region) A
NATURAL JOIN
(SELECT Region, TOB2, TOB3
FROM
(SELECT Region, count(Time_Of_Birth) as TOB2
FROM person 
WHERE Time_Of_Birth BETWEEN '08:01:00' AND '15:00:00'
GROUP BY Region) B
NATURAL JOIN
(SELECT Region, count(Time_Of_Birth) as TOB3
FROM person 
WHERE Time_Of_Birth BETWEEN '15:01:00' AND '22:59:00'
GROUP BY Region) C) D;