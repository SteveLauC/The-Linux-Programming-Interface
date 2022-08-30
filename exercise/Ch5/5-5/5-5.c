#include "../../../lib/tlpi_hdr.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int fd1 = open("tmp", O_CREAT | O_WRONLY, 0666);
	if (fd1 == -1) {
		errExit("open");
	}

	int fd2 = dup(fd1);
	if (fd2 == -1) {
		errExit("dup");
	}

	off_t offset1 = lseek(fd1, 0, SEEK_CUR);
	if (offset1 == -1) {
		errExit("lseek");
	}

	off_t offset2 = lseek(fd2, 0, SEEK_CUR);
	if (offset2 == -1) {
		errExit("lseek");
	}

	printf("before modification: offset1: %ld | offset2: %ld\n", offset1,
	       offset2);

	printf("writing 5 bytes to fd1...\n");
	ssize_t num_written = write(fd1, "hello", 5);
	if (num_written == -1) {
		errExit("write");
	}

	offset1 = lseek(fd1, 0, SEEK_CUR);
	if (offset1 == -1) {
		errExit("lseek");
	}

	offset2 = lseek(fd2, 0, SEEK_CUR);
	if (offset2 == -1) {
		errExit("lseek");
	}
	printf("after modification: offset1: %ld | offset2: %ld\n\n", offset1,
	       offset2);

	int flag1 = fcntl(fd1, F_GETFL);
	if (flag1 == -1) {
		errExit("fcntl get1 fd 1");
	}
	int flag2 = fcntl(fd2, F_GETFL);
	if (flag2 == -1) {
		errExit("fcntl get1 fd 2");
	}
	printf("before modification: flag1 has O_NONBLOCK %d| flag2 has O_NONBLOCK %d\n",
	       (flag1 & O_NONBLOCK) != 0, (flag2 & O_NONBLOCK) != 0);

	printf("setting O_NONBLOCK flag to fd1...\n");
	flag1 |= O_NONBLOCK;
	int res = fcntl(fd1, F_SETFL, flag1);
	if (res == -1) {
		errExit("fcntl set 1");
	}

	flag1 = fcntl(fd1, F_GETFL);
	if (flag1 == -1) {
		errExit("fcntl get2 fd 1");
	}
	flag2 = fcntl(fd2, F_GETFL);
	if (flag2 == -1) {
		errExit("fcntl get2 fd 2");
	}
	printf("after modification: flag1 has O_NONBLOCK %d| flag2 has O_NONBLOCK %d\n",
	       (flag1 & O_NONBLOCK)!=0, (flag2 & O_NONBLOCK)!=0);

	return EXIT_SUCCESS;
}
