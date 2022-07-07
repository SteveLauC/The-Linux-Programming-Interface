#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "get_num.h"

/*
 * purpose: error handling for `get_num.c`
 *
 * action: print `fname`, `name`, `msg` and `arg` infomation to stderr, the exit.
 *
 * argument: 
 *  * `fname`: function name, `getLong` or `getInt`
 *  * `msg`: description of the error case
 *  * `arg`: string containing a number
 *  * `name`: if `name` is not NULL, it should contain a string identifying the argument in `arg`
 *
 * note: only available in `get_num.c`
*/
static void gnFail(const char *fname, const char *msg, const char *arg,
		   const char *name)
{
	fprintf(stderr, "%s error", fname);

	if (name != NULL) {
		fprintf(stderr, "(in %s)", name);
	}

	fprintf(stderr, ": %s\n", msg);

	if (arg != NULL && *arg != '\0') {
		fprintf(stderr, "     offending text: %s", arg);
	}
	exit(EXIT_FAILURE);
}

/*
 * purpose: underlying number parsing function
 *
 * action: call `strtol`, and do some error handling after it
 *
 * arguments:
 *  * `fname`: function name, `getLong` or `getInt`
 *  * `arg`: string containing a number
 *  * `flags`: provides some control over the operation
 *  * `name`: if `name` is not NULL, it should contain a string identifying the argument in `arg`
 *
 *  return: a long integer
 *
 *  note: only available in `get_num.c`
*/
static long getNum(const char *fname, const char *arg, int flags,
		   const char *name)
{
	long res = 0;
	// `endptr` will be the address of the first invalid char after the call of `strtol`
	char *endptr = NULL;
	int base = 10;

	if (arg == NULL || *arg == '\0') {
		gnFail(fname, "null or emtpy string", arg, name);
	}

	base = (flags & GN_ANY_BASE) ? 0 :
	       (flags & GN_BASE_8)   ? 8 :
	       (flags & GN_BASE_16)  ? 16 :
					     10;

	// strtol(3) can return -1 on success, so we need to reset `errno` to 0 before
	// diagnosing error
	errno = 0;
	res = strtol(arg, &endptr, base);

	// error handling
	if (errno != 0) {
		gnFail(fname, "strtol() failed", arg, name);
	}
	if (*endptr != '\0') {
		gnFail(fname, "nonnumeric characters", arg, name);
	}
	if ((flags & GN_NONNEG) && res < 0) {
		gnFail(fname, "negative value not allowed", arg, name);
	}
	if ((flags & GN_GT_0) && res <= 0) {
		gnFail(fname, "value must be >= 0", arg, name);
	}

	return res;
}

/*
 * purpose: parse a long integer from a string
 *
 * action: call `getNum`
 * 
 * arguments:
 *  * `arg`: string containing a long integer value
 *  * `flags`: provides some control over the operation
 *  * `name`: if `name` is not NULL, it should contain a string identifying 
 *      the argument in `arg`
 *
 *  return: a long integer parsed from `arg`
*/
long getLong(const char *arg, int flags, const char *name)
{
	return getNum("getLong", arg, flags, name);
}

/*
 * purpose: parse an integer from a string
 *
 * action: call `getNum`
 * 
 * arguments:
 *  * `arg`: string containing an integer value
 *  * `flags`: provides some control over the operation
 *  * `name`: if `name` is not NULL, it should contain a string identifying the argument in `arg`
 *
 *  return: an integer parsed from `arg`
*/
int getInt(const char *arg, int flags, const char *name)
{
	long res_long = getNum("getInt", arg, flags, name);
	if (res_long > INT_MAX || res_long < INT_MIN) {
		gnFail("getInt", "integer out of range", arg, name);
	}

	return (int)res_long;
}
