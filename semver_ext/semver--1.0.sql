-- semver--1.0.sql

CREATE TYPE semver;

CREATE FUNCTION semver_in(cstring)
RETURNS semver
AS 'MODULE_PATHNAME', 'semver_in'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION semver_out(semver)
RETURNS cstring
AS 'MODULE_PATHNAME', 'semver_out'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE semver (
    INPUT = semver_in,
    OUTPUT = semver_out,
    INTERNALLENGTH = 12,
    ALIGNMENT = int4
);
-- Phase 2: Ezzah -- comparison functions and operators

CREATE FUNCTION semver_lt(semver, semver)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'semver_lt'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
    LEFTARG = semver,
    RIGHTARG = semver,
    PROCEDURE = semver_lt,
    COMMUTATOR = >,
    NEGATOR = >=
);

CREATE FUNCTION semver_le(semver, semver)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'semver_le'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
    LEFTARG = semver,
    RIGHTARG = semver,
    PROCEDURE = semver_le,
    COMMUTATOR = >=,
    NEGATOR = >
);

CREATE FUNCTION semver_eq(semver, semver)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'semver_eq'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
    LEFTARG = semver,
    RIGHTARG = semver,
    PROCEDURE = semver_eq,
    COMMUTATOR = =,
    NEGATOR = <>
);
