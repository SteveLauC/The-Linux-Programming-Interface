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
