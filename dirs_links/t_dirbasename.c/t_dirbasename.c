#include <libgen.h>
#include "../../lib/tlpi_hdr.h"

int main(int ac, char *av[])
{
	char *t1, *t2;
	int j;

	for (j = 1; j < ac; j += 1) {
		t1 = strdup(av[j]);
		if (t1 == NULL) {
			errExit("strdup");
		}

		t2 = strdup(av[j]);
		if (t2 == NULL) {
			errExit("strdup");
		}

		printf("%s ==> %s + %s\n", av[j], dirname(t1), basename(t2));
		free(t1);
		free(t2);
	}
}
