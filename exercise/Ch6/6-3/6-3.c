/*
 * QUESTION: Implement `setenv()` and `unsetenv()` using `getenv()`, `putenv()`,
 * and, where necessary, code that directly modifies `environ`. Your version of
 * `unsetenv()` should check to see whether there are multiple definitons of an
 * environment variable, and remove them all (which is what glibc version of 
 * `unsetenv()` does).
 *
 *
 * TEST:
 * $ gcc 6-3.c ../../../lib/error_functions.c -0 6-3
 * $ ./6-3
 * $ echo $?
 * 0   # all tests passed
 *
 * NOTE: If the argumnet of `putenv()` does not have the format of "name=value",
 * it will simply return 0 :(
 * And the memory allocated in `allocation()` is not freed. This book says the
 * official `setenv()` also have memory leak issue, I tested it and did not any
 * memory leak.
 *
 * You can use `valgrind --leak-check=yes ./6-3` to check memory leak
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../lib/tlpi_hdr.h"

extern char **environ;

int my_setenv(const char *name, const char *value, int replace);
int my_unsetenv(const char *name);
static int allocation(const char *name, const char *value);

int main(void)
{
	// int res = my_setenv("a", "b", 0);
	// printf("return value: %d\n", res);
	// char **ep = environ;
	// if (ep != NULL) {
	// 	while (*ep != NULL) {
	// 		printf("%s\n", *ep);
	// 		ep += 1;
	// 	}
	// }
	// printf("\n\n");
	// const char *p = getenv("a");
	// if (p != NULL) {
	// 	printf("value: %s\n", p);
	// }
	// return 0;

	assert(my_setenv("a", "a", 0) == 0);
	const char *value_of_a = getenv("a");
	assert(value_of_a != NULL);

	assert(my_setenv("a", "b", 0) == 0);
	value_of_a = getenv("a");
	assert(value_of_a != NULL);
	assert(strcmp(getenv("a"), "a") == 0);

	assert(my_setenv("a", "b", 1) == 0);
	value_of_a = getenv("a");
	assert(value_of_a != NULL);
	assert(strcmp(getenv("a"), "b") == 0);

	assert(my_unsetenv("a") == 0);
	value_of_a = getenv("a");
	assert(value_of_a == NULL);
}

static int allocation(const char *name, const char *value)
{
	// name=value'\0'
	unsigned long len = strlen(name) + strlen(value) + 2;
	// overflow check, useful on 32-bit system
	if (len > (size_t)-1) {
		errExit("environment variable too big");
	}
	char *new_env = (char *)malloc((size_t)len);

	strncpy(new_env, name, strlen(name)); // copy the name itself
	strncpy(new_env + strlen(name), "=", 1);
	// NUL included
	strncpy(new_env + strlen(name) + 1, value, strlen(value) + 1);

	int putenv_res = putenv(new_env);
	return putenv_res;
}

int my_setenv(const char *name, const char *value, int replace)
{
	if (name == NULL || *name == '\0' || strchr(name, '=') != NULL) {
		errno = EINVAL;
		return -1;
	}

	// we need to delete it first
	if (getenv(name) != NULL) {
		if (replace != 0) {
			unsetenv(name);
			return allocation(name, value);
		} else {
			return 0;
		}
	} else {
		return allocation(name, value);
	}
}

int my_unsetenv(const char *name)
{
	if ((name == NULL) | (*name == '\0')) {
		errno = EINVAL;
		return -1;
	}
	char **ep = environ;
	if (ep != NULL) {
		while (*ep != NULL) {
			// find it
			if (strncmp(*ep, name, strlen(name)) == 0) {
				char **dp = ep;
				do {
					dp[0] = dp[1];
				} while (*(dp++));
			}

			ep += 1;
		}
	}

	return 0;
}
