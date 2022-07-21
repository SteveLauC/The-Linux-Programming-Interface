#include "../lib/tlpi_hdr.h"

extern char **environ;

int main(void)
{
	char **ep = NULL;

	for (ep = environ; *ep != NULL; ep += 1) {
		puts(*ep);
	}

	exit(EXIT_SUCCESS);
}
