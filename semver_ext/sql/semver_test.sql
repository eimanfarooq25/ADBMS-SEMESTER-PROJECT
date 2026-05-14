-- =============================================
-- Regression Tests for semver Extension
-- =============================================

CREATE EXTENSION IF NOT EXISTS semver;

-- Basic input/output
SELECT '1.2.3'::semver;
SELECT '0.0.1'::semver;
SELECT '16.4.0'::semver;

-- Invalid inputs
SELECT '1.2.x'::semver;
SELECT '1.2'::semver;
SELECT '1.2.3.4'::semver;
SELECT 'v1.2.3'::semver;

-- Comparison operators
SELECT '1.10.0'::semver > '1.9.0'::semver;
SELECT '1.2.3'::semver < '1.10.0'::semver;
SELECT '1.2.3'::semver <= '1.2.3'::semver;
SELECT '1.2.3'::semver = '1.2.3'::semver;
SELECT '2.0.0'::semver >= '1.9.9'::semver;

-- Utility Functions (Student C)
SELECT major('16.4.0'::semver);
SELECT minor('16.4.0'::semver);
SELECT patch('16.4.0'::semver);
SELECT bump_minor('1.2.3'::semver);
SELECT is_compatible('1.5.0'::semver, '1.4.7'::semver);
SELECT is_compatible('2.0.0'::semver, '1.9.0'::semver);

-- Real world usage (without ORDER BY to avoid operator class issue)
CREATE TABLE sv_test (name text, version semver);
INSERT INTO sv_test VALUES 
('postgres', '16.4.0'),
('node', '20.10.0'),
('nginx', '1.27.1'),
('redis', '7.2.5');

SELECT * FROM sv_test;
SELECT name, version, is_compatible(version, '1.0.0'::semver) FROM sv_test;

DROP TABLE sv_test;
