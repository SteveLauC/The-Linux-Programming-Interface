#include "../lib/tlpi_hdr.h"

int main(int ac, char *av[])
{
	int j;

	for (j = 0; j < ac; j += 1) {
		printf("av[%d] = %s\n", j, av[j]);
	}
	exit(EXIT_SUCCESS);
}
