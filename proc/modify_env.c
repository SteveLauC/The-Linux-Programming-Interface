/*
 * compile: gcc modify_env.c ../lib/error_functions.c -o modify_env
*/

#include "../lib/tlpi_hdr.h"

extern char **environ;

int main(int ac, char *av[])
{
	char **ep = NULL;

	clearenv(); /* Erase entire environment */

	for (int j = 1; j < ac; j += 1) {
		if (putenv(av[j]) != 0) {
			errExit("putenv: %s", av[j]);
		}
	}

	if (setenv("GREET", "Hello world", 0) == -1) {
		errExit("setenv");
	}

	unsetenv("BYE");

	for (ep = environ; *ep != NULL; ep += 1) {
		puts(*ep);
	}

	exit(EXIT_SUCCESS);
}
