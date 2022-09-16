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
	ORDER BY timestart DESC LIMIT 1;

	SELECT gmtoffset INTO @targetgmtoffset
	FROM time_zone
	WHERE timestart <= UNIX_TIMESTAMP(sourcedatetimestamp) AND timezonecode=targettimezonecode
	ORDER BY timestart DESC LIMIT 1;

    SELECT FROM_UNIXTIME(UNIX_TIMESTAMP(sourcedatetimestamp)-(@srcgmtoffset)+(@targetgmtoffset)) INTO targetdatetimestamp;
    RETURN targetdatetimestamp;
END$$
DELIMITER ;

#'YYYY-MM-DD hh:mm:ss' 
SELECT timezoneconvert('2022-07-29 02:53:00', 'EST', 'IST');
