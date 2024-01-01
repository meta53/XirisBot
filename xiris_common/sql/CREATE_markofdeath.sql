------------------------------
-- Curing (General)
------------------------------
-- DROP TABLE IF EXISTS Curing;
-- CREATE TABLE IF NOT EXISTS Curing(
-- SpawnID INTEGER UNIQUE PRIMARY KEY,
-- Name TEXT UNIQUE,
-- Counters TINYINT NOT NULL,
-- CurerName TEXT DEFAULT NULL,
-- CurerID INTEGER DEFAULT 0);
-- 

------------------------------
-- Mark of Death (Specific)
------------------------------
-- 
-- DROP TABLE IF EXISTS MarkOfDeath;
-- CREATE TABLE IF NOT EXISTS MarkOfDeath(
-- SpawnID INTEGER UNIQUE PRIMARY KEY,
-- Name TEXT UNIQUE,
-- Counters TINYINT NOT NULL,
-- CurerName TEXT DEFAULT NULL,
-- CurerID INTEGER DEFAULT 0,
-- ZoneID INTEGER DEFAULT 0);

--INSERT OR IGNORE INTO MarkOfDeath(SpawnID,Counters) VALUES (402,19);
--INSERT OR REPLACE INTO MarkOfDeath(SpawnID, Name ,Counters,CurerName,CurerID) VALUES (22, 'Xiris', 14, NULL, 0);
SELECT * FROM MarkOfDeath;
