/*
 * QUESTION: modify the program in listing 7-1(free_and_sbrk.c) to print out the
 * current value of the program break after each execution of `malloc()`. Run the
 * program specifying a small allocation block size. This will demonstrate that
 * `malloc()` doesn't employ `sbrk()` to adjust the program break on each call, 
 * but instead periodically allocates larger chunks of memory from which it passes
 * back small pieces to the caller.
 *
 * TEST:
 * $ gcc 7-1.c ../../../lib/error_functions.c ../../../lib/get_num.c -o 7-1
 * $ ./7-1 20 10240 > result
 * $ cat result
 * Initial program break:       0x55cc3977f000
 * Allocating 20*10240 bytes
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc3977f000
 * (During allocation)Program break is:       0x55cc397a0000
 * (During allocation)Program break is:       0x55cc397a0000
 * (During allocation)Program break is:       0x55cc397a0000
 * (During allocation)Program break is:       0x55cc397a0000
 * (During allocation)Program break is:       0x55cc397a0000
 * (During allocation)Program break is:       0x55cc397a0000
 * (During allocation)Program break is:       0x55cc397a0000
 * (During allocation)Program break is:       0x55cc397a0000
 * Program break is now:       0x55cc397a0000
 * Freeing blocks from 1 to 20 in steps of 1
 * After free(), program break is       0x55cc39780000
*/

#include "../../../lib/tlpi_hdr.h"

#define MAX_ALLOCS 10000

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

		printf("(During allocation)Program break is: %20p\n", sbrk(0));
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
