#include <fcntl.h>
#include "../lib/tlpi_hdr.h"

#define MAX_LEN 100

int main(int ac, char *av[])
{
	int fd = open("/proc/sys/kernel/pid_max", (ac > 1) ? O_RDWR : O_RDONLY);
	if (fd == -1) {
		errExit("open");
	}

	char buf[MAX_LEN];
	ssize_t n = read(fd, buf, MAX_LEN);
	if (n == -1) {
		errExit("read");
	}

	if (ac > 1) {
		printf("Old value: ");
	}
	printf("%.*s", (int)n, buf);

	if (ac > 1) {
		if (lseek(fd, 0, SEEK_SET) == -1) {
			errExit("lseek");
		}
		if (write(fd, av[1], strlen(av[1])) != (ssize_t)strlen(av[1])){
			errExit("write");
		}

		system("echo /proc/sys/kernel/pid_max now contains \
		       `cat /proc/sys/kernel/pid_max`");
	}
	exit(EXIT_SUCCESS);
}
