/*
 * Question: modify the program in Listing 5-3 to use standard file I/O system calls
 * (open() and lseek()) and `off_t` data type. Compile the program with the 
 * `_FILE_OFFSET_BITS` macro set to 64, and test it to show that a large file can
 * be successfully created.
 *
 * Build:
 * $ gcc 5-1.c ../../../lib/error_functions.c -o 5-1
 *
 * Test:
 * $ ./5-1 test 3221225472
 * $ ls -l test
 * -rw------- 1 steve steve 3221225476 Jul 17 10:03 test
 */

#define _FILE_OFFSET_BITS 64
#include <sys/stat.h>
#include <fcntl.h>
#include "../../../lib/tlpi_hdr.h"

int main(int argc, char *argv[])
{
	int fd;
	off_t off;

	if (argc != 3 || strcmp(argv[1], "--help") == 0)
		usageErr("%s pathname offset\n", argv[0]);

	fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
		errExit("open64");

	off = atoll(argv[2]);
	if (lseek(fd, off, SEEK_SET) == -1)
		errExit("lseek64");

	if (write(fd, "test", 4) == -1)
		errExit("write");
	exit(EXIT_SUCCESS);
}
