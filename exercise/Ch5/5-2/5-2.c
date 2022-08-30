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
