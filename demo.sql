-- =============================================
-- DEMO.SQL - Final Demonstration for semver Extension
-- =============================================

CREATE EXTENSION IF NOT EXISTS semver;

-- Sample Data
CREATE TABLE packages (
    name text PRIMARY KEY,
    version semver NOT NULL
);

INSERT INTO packages VALUES
('postgres', '16.4.0'),
('node',    '20.10.0'),
('redis',   '7.2.5'),
('nginx',   '1.27.1'),
('rust',    '1.81.0'),
('ubuntu',  '24.04.0');

-- 1. Basic Usage
SELECT '1.2.3'::semver AS basic_test;

-- 2. Comparison Operators
SELECT '1.10.0'::semver > '1.9.0'::semver AS greater_test;

-- 3. Utility Functions (Your Part)
SELECT 
    name,
    version,
    major(version) AS major,
    minor(version) AS minor,
    patch(version) AS patch
FROM packages 
WHERE name = 'postgres';

SELECT bump_minor('1.2.3'::semver) AS bumped_version;

-- 4. is_compatible Function
SELECT 
    name, 
    version, 
    is_compatible(version, '1.0.0'::semver) AS compatible_with_1_0
FROM packages;

-- 5. Filtering (without ORDER BY)
SELECT name, version 
FROM packages 
WHERE version >= '2.0.0'::semver;

-- 6. Note about Aggregates & Sorting
SELECT 'Note: MAX/MIN and ORDER BY need aggregate + operator class (bonus feature)' AS note;
