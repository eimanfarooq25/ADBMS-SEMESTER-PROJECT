#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

typedef struct {
    int32 major;
    int32 minor;
    int32 patch;
} Semver;

PG_FUNCTION_INFO_V1(semver_in);
Datum
semver_in(PG_FUNCTION_ARGS)
{
    char   *str = PG_GETARG_CSTRING(0);
    Semver *result;
    int32   major, minor, patch;
    int     n;
    char    dummy;

    if (strchr(str, '-') || strchr(str, '+'))
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input syntax for type semver: \"%s\"", str)));

    n = sscanf(str, "%d.%d.%d", &major, &minor, &patch);
    if (n != 3)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input syntax for type semver: \"%s\"", str)));

    if (sscanf(str, "%d.%d.%d%c", &major, &minor, &patch, &dummy) != 3)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input syntax for type semver: \"%s\"", str)));

    if (major < 0 || minor < 0 || patch < 0)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input syntax for type semver: \"%s\"", str)));

    result = (Semver *) palloc(sizeof(Semver));
    result->major = major;
    result->minor = minor;
    result->patch = patch;

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(semver_out);

Datum semver_out(PG_FUNCTION_ARGS)
{
    Semver *sv = (Semver *) PG_GETARG_POINTER(0);

    char *buf = (char *) palloc(32);

    snprintf(buf, 32, "%d.%d.%d",
             sv->major, sv->minor, sv->patch);

    PG_RETURN_CSTRING(buf);
}
/* semver_compare: internal helper, not registered in SQL */
static int
semver_compare(Semver *a, Semver *b)
{
    if (a->major != b->major)
        return (a->major > b->major) ? 1 : -1;
    if (a->minor != b->minor)
        return (a->minor > b->minor) ? 1 : -1;
    if (a->patch != b->patch)
        return (a->patch > b->patch) ? 1 : -1;
    return 0;
}

/* semver_lt: less than */
PG_FUNCTION_INFO_V1(semver_lt);
Datum semver_lt(PG_FUNCTION_ARGS)
{
    Semver *a = (Semver *) PG_GETARG_POINTER(0);
    Semver *b = (Semver *) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(semver_compare(a, b) < 0);
}

/* semver_le: less than or equal */
PG_FUNCTION_INFO_V1(semver_le);
Datum semver_le(PG_FUNCTION_ARGS)
{
    Semver *a = (Semver *) PG_GETARG_POINTER(0);
    Semver *b = (Semver *) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(semver_compare(a, b) <= 0);
}

/* semver_eq: equal */
PG_FUNCTION_INFO_V1(semver_eq);
Datum semver_eq(PG_FUNCTION_ARGS)
{
    Semver *a = (Semver *) PG_GETARG_POINTER(0);
    Semver *b = (Semver *) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(semver_compare(a, b) == 0);
}

/* semver_ne: not equal */
PG_FUNCTION_INFO_V1(semver_ne);
Datum semver_ne(PG_FUNCTION_ARGS)
{
    Semver *a = (Semver *) PG_GETARG_POINTER(0);
    Semver *b = (Semver *) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(semver_compare(a,b) != 0);
}

/* semver_ge: greater than or equal */
PG_FUNCTION_INFO_V1(semver_ge);
Datum semver_ge(PG_FUNCTION_ARGS)
{
    Semver *a = (Semver *) PG_GETARG_POINTER(0);
    Semver *b = (Semver *) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(semver_compare(a,b) >= 0);
}

/* semver_gt: greater than */
PG_FUNCTION_INFO_V1(semver_gt);
Datum semver_gt(PG_FUNCTION_ARGS)
{
    Semver *a = (Semver *) PG_GETARG_POINTER(0);
    Semver *b = (Semver *) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(semver_compare(a,b) > 0);
}

/* semver_cmp: returns integer for sort ordering */
PG_FUNCTION_INFO_V1(semver_cmp);
Datum semver_cmp(PG_FUNCTION_ARGS)
{
    Semver *a = (Semver *) PG_GETARG_POINTER(0);
    Semver *b = (Semver *) PG_GETARG_POINTER(1);
    PG_RETURN_INT32(semver_compare(a,b));
}


PG_FUNCTION_INFO_V1(semver_major);
Datum
semver_major(PG_FUNCTION_ARGS)
{
    Semver *sv = (Semver *) PG_GETARG_POINTER(0);
    PG_RETURN_INT32(sv->major);
}

PG_FUNCTION_INFO_V1(semver_minor);
Datum
semver_minor(PG_FUNCTION_ARGS)
{
    Semver *sv = (Semver *) PG_GETARG_POINTER(0);
    PG_RETURN_INT32(sv->minor);
}

PG_FUNCTION_INFO_V1(semver_patch);
Datum
semver_patch(PG_FUNCTION_ARGS)
{
    Semver *sv = (Semver *) PG_GETARG_POINTER(0);
    PG_RETURN_INT32(sv->patch);
}

PG_FUNCTION_INFO_V1(bump_minor);
Datum
bump_minor(PG_FUNCTION_ARGS)
{
    Semver *sv = (Semver *) PG_GETARG_POINTER(0);
    Semver *result = (Semver *) palloc(sizeof(Semver));
    result->major = sv->major;
    result->minor = sv->minor + 1;
    result->patch = 0;
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(is_compatible);
Datum
is_compatible(PG_FUNCTION_ARGS)
{
    Semver *a = (Semver *) PG_GETARG_POINTER(0);
    Semver *b = (Semver *) PG_GETARG_POINTER(1);

    if (a->major != b->major)
        PG_RETURN_BOOL(false);
    if (b->minor > a->minor)
        PG_RETURN_BOOL(false);
    if (b->minor == a->minor && b->patch > a->patch)
        PG_RETURN_BOOL(false);

    PG_RETURN_BOOL(true);
}
