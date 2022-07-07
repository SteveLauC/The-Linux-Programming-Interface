/*
 * compile: gcc copy.c ../lib/error_functions.c -o copy
*/
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../lib/tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int ac, char *av[])
{
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if (ac != 3) {
		usageErr("%s old-file new-file\n", av[0]);
	}

	inputFd = open(av[1], O_RDONLY);
	if (inputFd == -1) {
		errExit("opening file %s", av[1]);
	}

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = 0666;
	outputFd = open(av[2], openFlags, filePerms);
	if (outputFd == -1) {
		errExit("opening file %s", av[2]);
	}

	// transfer data until we encounter EOF or an error

	while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
		if (write(outputFd, buf, numRead) != numRead) {
			fatal("couldn't write whole buffer");
		}
	}

	if (numRead == -1) {
		errExit("read");
	}

	if (close(inputFd) == -1) {
		errExit("close input");
	}

	if (close(outputFd) == -1) {
		errExit("close output");
	}

	exit(EXIT_SUCCESS);
}
