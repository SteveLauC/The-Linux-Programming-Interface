#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../lib/tlpi_hdr.h"

int main(int ac, char *av[])
{
	int fd = open(av[1], O_WRONLY); // Open 1: check if file exists

	if (fd != -1) { // Open successed
		printf("[PID %ld] File \"%s\" already exists\n", (long)getpid(),
		       av[1]);
		close(fd);
	} else {
		// Fail for unexpected reason
		if (errno != ENOENT) {
			errExit("open");
		} else {
			// suspend to simulate time gap between processes scheduling
			// executing like `./a.out file sleep&` to suspend current process
			printf("[PID %ld] File \"%s\" doesn't exist yet\n",
			       (long)getpid(), av[1]);
			if (ac > 2) {
				sleep(5);
				printf("[PID %ld] Done sleeping\n",
				       (long)getpid());
			}

			fd = open(av[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
			if (fd == -1) {
				errExit("open");
			}

			printf("[PID %ld] Created file \"%s\" exclusively\n",
			       (long)getpid(), av[1]);
		}
	}

	return 0;
}
