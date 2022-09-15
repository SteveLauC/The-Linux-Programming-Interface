/*
 * usage: direct_read FILE NUM_BYTES_TO_READ [OFFSET] [BUFF_ALIGNMENT]
 */
#define _GNU_SOURCE

#include <fcntl.h>
#include "../lib/tlpi_hdr.h"

int main(int ac, char *av[])
{
	if (ac < 3) {
		usageErr("%s FILE NUM_BYTES_TO_READ [OFFSET] [BUFF_ALIGNMENT]");
	}

	int64_t num_bytes_to_read =
		getLong(av[2], GN_ANY_BASE, "num_bytes_to_read");
	int64_t offset = (ac > 3) ? getLong(av[3], GN_ANY_BASE, "offset") : 0;
	int64_t alignment =
		(ac > 4) ? getLong(av[4], GN_ANY_BASE, "alignment") : 4096;

	int fd = open(av[1], O_RDONLY | O_DIRECT);
	if (fd == -1) {
		perror("open(2)");
		errExit("open");
	}

	char *ptr = NULL;
	int res = posix_memalign((void **)&ptr, alignment, num_bytes_to_read);
	if (res != 0) {
		printf("error code of posix_memalign: %d\n", res);
		errExit("posix_memalign");
	}

	if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
		perror("lseek(2)");
		errExit("lseek");
	}

	ssize_t num_read = read(fd, ptr, (size_t)num_bytes_to_read);
	if (num_read == -1) {
		perror("read(2)");
		errExit("read");
	}
	printf("Read %ld bytes\n", (long)num_read);

	exit(EXIT_SUCCESS);
}
