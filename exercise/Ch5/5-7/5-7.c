#include "../../../lib/tlpi_hdr.h"
#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Bytes {
	void *base;
	size_t len;
};

ssize_t my_readv(int fd, struct Bytes *array, size_t array_len);
ssize_t my_writev(int fd, struct Bytes *array, size_t array_len);

int main(void)
{
	int source = open("source", O_RDONLY);
	if (source == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	char buf1[10];
	char buf2[10];

	struct Bytes buffers[2];
	buffers[0].base = buf1;
	buffers[0].len = 10;
	buffers[1].base = buf2;
	buffers[1].len = 10;

	ssize_t num_read = my_readv(source, buffers, 2);
	close(source);

	printf("request 20 bytes, read %ld bytes\n", num_read);

	ssize_t num_written = my_writev(1, buffers, 2);
	printf("write %ld bytes to stdout\n", num_written);
	return EXIT_SUCCESS;
}

ssize_t my_readv(int fd, struct Bytes *array, size_t array_len)
{
	size_t total_num_read = 0;
	size_t num_requested = 0;
	for (size_t i = 0; i < array_len; i += 1) {
		num_requested += array[i].len;
		int num_read = read(fd, array[i].base, array[i].len);
		if (-1 == num_read) {
			return -1;
		}
		total_num_read += num_read;
	}

	return (ssize_t)total_num_read;
}

ssize_t my_writev(int fd, struct Bytes *array, size_t array_len)
{
	size_t total_num_written = 0;
	size_t num_requested = 0;
	for (size_t i = 0; i < array_len; i += 1) {
		num_requested += array[i].len;
		int num_written = write(fd, array[i].base, array[i].len);
		if (-1 == num_written) {
			return -1;
		}

		total_num_written += num_written;
	}

	return (ssize_t)total_num_written;
}
