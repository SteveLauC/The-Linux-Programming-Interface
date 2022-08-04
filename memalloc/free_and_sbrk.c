#include "../lib/tlpi_hdr.h"

#define MAX_ALLOCS 1000000

int main(int ac, char *av[])
{
	char *ptr[MAX_ALLOCS];

	int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

	printf("\n");

	if (ac < 3 || strcmp(av[1], "--help") == 0) {
		usageErr("%s num-allocs block-size [step [min [max]]]\n",
			 av[0]);
	}

	numAllocs = getInt(av[1], GN_GT_0, "num-allocs");

	if (numAllocs > MAX_ALLOCS) {
		cmdLineErr("num-allocs > %d", MAX_ALLOCS);
	}

	blockSize = getInt(av[2], GN_GT_0, "block-size");

	freeStep = (ac > 3) ? getInt(av[3], GN_GT_0, "free-step") : 1;
	freeMin = (ac > 4) ? getInt(av[4], GN_GT_0, "free-min") : 1;
	freeMax = (ac > 5) ? getInt(av[5], GN_GT_0, "free-max") : numAllocs;

	if (freeMax > numAllocs) {
		cmdLineErr("free-max > num-allocs");
	}

	printf("Initial program break: %20p\n", sbrk(0));
	printf("Allocating %d*%d bytes\n", numAllocs, blockSize);

	for (j = 0; j < numAllocs; j += 1) {
		ptr[j] = (char *)malloc(blockSize);
		if (ptr[j] == NULL) {
			errExit("malloc");
		}
	}

	printf("Program break is now: %20p\n", sbrk(0));

	printf("Freeing blocks from %d to %d in steps of %d\n", freeMin,
	       freeMax, freeStep);

	for (j = freeMin - 1; j < freeMax; j += freeStep) {
		free(ptr[j]);
	}

	printf("After free(), program break is %20p\n", sbrk(0));

	exit(EXIT_SUCCESS);
}
