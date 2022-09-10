#Q2: Import data from timezone.csv and country.csv into tables called “time_zone” (zonename,
#countrycode, timezonecode, timestart, gmtoffset, dst) and “country” (countrycode,
#countryname) in your local mysql database. Using the data from the database, write a user-
#defined function called “timezoneconvert” which takes three arguments - sourcedatetimestamp,
#sourcetimezonecode and targettimezone. It should return the targetdatetimestamp. Ignore
#DaylightSaving timezones here. (10 Marks)
#Example: timezoneconvert(‘29-07-2022 02:53:00’, ‘EST’, ‘IST’) - 29-07-2022 12:23:00
DROP TABLE country;
CREATE TABLE IF NOT EXISTS country (
    countrycode VARCHAR(255) NOT NULL,
    countryname VARCHAR(255) NOT NULL
);

DROP TABLE time_zone;
CREATE TABLE IF NOT EXISTS time_zone(
    zonename VARCHAR(255) NOT NULL,
    countrycode VARCHAR(255) NOT NULL,
    timezonecode VARCHAR(255) NOT NULL,
    timestart BIGINT NOT NULL,
    gmtoffset INT NOT NULL,
    dst INT NOT NULL
);

SELECT * FROM country;
SELECT * FROM time_zone;
-- -----------------------------------------------------------------------------------------------------------

SELECT FROM_UNIXTIME(UNIX_TIMESTAMP(('1998-05-06 08:00:00')) - `gmtoffset`) into @var1 
FROM `time_zone`
WHERE `timestart` <= UNIX_TIMESTAMP('1998-05-06 08:00:00') AND `timezonecode` = 'IST'
ORDER BY `timestart` ASC LIMIT 1;

SELECT timestart, UNIX_TIMESTAMP(@var1), FROM_UNIXTIME(UNIX_TIMESTAMP(@var1) + `gmtoffset`) AS `local_time`
FROM `time_zone`
WHERE `timestart` <= UNIX_TIMESTAMP(@var1) AND `timezonecode` = 'YST'
ORDER BY `timestart` ASC LIMIT 1;

-- --------------------------------------------------------------------------------------------------------------
DROP FUNCTION timezoneconvert;
DELIMITER $$
CREATE FUNCTION timezoneconvert(
    sourcedatetimestamp DATETIME,  
    sourcetimezonecode VARCHAR(20),
    targettimezonecode VARCHAR(20)
)
RETURNS DATETIME
DETERMINISTIC
BEGIN
	DECLARE targetdatetimestamp DATETIME;

	SELECT gmtoffset INTO @srcgmtoffset
	FROM time_zone
	WHERE timestart <= UNIX_TIMESTAMP(sourcedatetimestamp) AND timezonecode=sourcetimezonecode
	ORDER BY timestart ASC LIMIT 1;

	SELECT gmtoffset INTO @targetgmtoffset
	FROM time_zone
	WHERE timestart <= UNIX_TIMESTAMP(sourcedatetimestamp) AND timezonecode=targettimezonecode
	ORDER BY timestart ASC LIMIT 1;

    SELECT FROM_UNIXTIME(UNIX_TIMESTAMP(sourcedatetimestamp)-(@srcgmtoffset)+(@targetgmtoffset)) INTO targetdatetimestamp;
    RETURN targetdatetimestamp;
END$$
DELIMITER ;

#'YYYY-MM-DD hh:mm:ss' 
SELECT timezoneconvert('1998-05-06 08:00:00', 'IST', 'YST');




