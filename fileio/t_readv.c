#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include "../lib/tlpi_hdr.h"

int main(int ac, char *av[])
{
	int fd;
	struct iovec iov[3];

	struct stat myStruct; // first buffer
	int x; // second buffer
#define STR_SIZE 100
	char str[STR_SIZE]; // third buffer

	ssize_t numRead, totRequired;

	if (ac != 2) {
		usageErr("%s file\n", av[0]);
	}

	fd = open(av[1], O_RDONLY);
	if (fd == -1) {
		errExit("open");
	}

	totRequired = 0;
	iov[0].iov_base = &myStruct;
	iov[0].iov_len = sizeof(struct stat);
	totRequired += iov[0].iov_len;

	iov[1].iov_base = &x;
	iov[1].iov_len = sizeof(x);
	totRequired += iov[1].iov_len;

	iov[2].iov_base = str;
	iov[2].iov_len = STR_SIZE;
	totRequired += iov[2].iov_len;

	numRead = readv(fd, iov, 3);
	if (numRead == -1) {
		errExit("readv");
	}

	if (numRead < totRequired) {
		printf("read fewer bytes than requested\n");
	}

	printf("total bytes requested: %ld; bytes read: %ld\n",
	       (long)totRequired, (long)numRead);
	exit(EXIT_SUCCESS);
}
