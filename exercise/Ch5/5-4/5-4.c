/*
 * QUESTION: Implement `dup()` and `dup2()` using `fcntl()` and, where necessary,
 * `close()`. (You may ignore the fact that dup2() and fcntl() return different
 * errno values for some error cases.) For `dup2()`, remeber to handle the spcial
 * case where oldfd equals newfd. In this case, you should check whether oldfd is 
 * valid, which can be done by, for example, checking if `fcntl(fd, F_GETFL)` 
 * succeeds. If oldfd is not valid, then the function should return -1 with errno
 * set to EBADF
 *
 *
 * TEST: In function main, I redirect stdout to the file specified by name av[1]
 * and stderr to file specified by av[2], then flush some bytes to stdout and stderr
 *
 * 	$ gcc 5-4.c ../../../lib/error_functions.c -o 5-4
 * 	$ ./5-4 stdout stderr
 * 	$ cat stdout
 * 	hello stdout
 *
 * 	$ cat stderr
 * 	hello stderr
 *
 *
 * ISSUE: In `my_dup2`, if `new_open_fd` does not equals to `newfd`, the function
 * will return `-1`, but errno will not be modified.
*/
#include <fcntl.h>
#include "../../../lib/tlpi_hdr.h"
#include <unistd.h>

int my_dup(int);
int my_dup2(int, int);

int main(int ac, char *av[])
{
	if (ac != 3) {
		usageErr("%s file1 file2\n");
	}
	// redirect stdout to a av[1]
	int file1 = open(av[1], O_CREAT | O_WRONLY, 0666);
	if (file1 == -1) {
		errExit("open");
	}
	close(1);
	my_dup(file1);
	printf("hello stdout\n");

	// redirect stdout to a av[2]
	int file2 = open(av[2], O_CREAT | O_WRONLY, 0666);
	if (file2 == -1) {
		errExit("open");
	}
	my_dup2(file2, 2);
	fprintf(stderr, "hello stderr\n");

	close(file1);
	close(file2);

	return EXIT_SUCCESS;
}

int my_dup(int oldfd)
{
	// if oldfd is not a valid fd, return -1 with errno set to `EBADF`
	errno = 0;
	if (-1 == fcntl(oldfd, F_GETFL) && errno == EBADF) {
		return -1;
	}

	return fcntl(oldfd, F_DUPFD, 0);
}

/*
 * Note: the steps of opening `newfd` and reusing it are NOT perfomed atomically
 */
int my_dup2(int oldfd, int newfd)
{
	// if oldfd is not a valid fd, return -1 with errno set to `EBADF`
	errno = 0;
	if (-1 == fcntl(oldfd, F_GETFL) && errno == EBADF) {
		return -1;
	}

	// if oldfd equals to newfd
	if (oldfd == newfd) {
		return newfd;
	}

	// if newfd is an open fd
	if (fcntl(newfd, F_GETFL) != -1) {
		// all errors encountered during this `close` operation are
		// ignored
		close(newfd);
	}

	int new_open_fd = fcntl(oldfd, F_DUPFD, newfd);
	if (new_open_fd == -1) {
		return -1;
	} else if (new_open_fd == newfd) {
		return newfd;
	} else {
		// new_open_fd > newfd
		close(new_open_fd);
		return -1;
	}
}
