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
