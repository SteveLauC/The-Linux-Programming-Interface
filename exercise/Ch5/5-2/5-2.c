/*
 * Question: write a program that opens an existing file for writing with the 
 * `O_APPEND` flag, and then seeks to the beginning of the file before writing
 * any data. Where does the data appear in the file? Why?
 *
 * Answer: The data will be written to the end of the file for the reason for
 * if a file is opened with `O_APPEND` flag, the offset will always be repositioned
 * to the end before any `write(2)` call.
 *
 * Build: gcc 5-2.c ../../../lib/error_functions.c -o 5-2
 *
 * Test:
 * $ echo "hello" > test
 * $ cat test
 * hello
 *
 * $ gcc 5-2.c ../../../lib/error_functions.c -o 5-2
 * $ ./5-2 test
 * $ cat test
 * hello
 * test
*/


#include "../../../lib/tlpi_hdr.h"
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>

int main(int ac, char *av[])
{
	if (ac < 2) {
		usageErr("%s path\n", av[0]);
	}

	/* make sure that the file exists and is not empty */
	struct stat buf;
	if (stat(av[1], &buf) != 0 || buf.st_size == 0) {
		errExit("path must be a non-empty valid file\n");
	}

	int fd = open(av[1], O_WRONLY | O_APPEND);
	if (fd == -1) {
		errExit("open");
	}

	/* Seek to the beginning of the file */
	assert(lseek(fd, 0, SEEK_SET) == 0);

	if (-1 == write(fd, "test", 4)) {
		errExit("write");
	}

	close(fd);
	return EXIT_SUCCESS;
}
