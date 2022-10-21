#include <fcntl.h>
#include "../../lib/tlpi_hdr.h"

#define CMD_SIZE 200
#define BUF_SIZE 1024

int main(int ac, char *av[])
{
	// Command to be passed to system(3)
	char shell_cmd[CMD_SIZE];
	// random bytes to write to file
	char buf[BUF_SIZE];

	if (ac < 2 || strcmp(av[1], "--help") == 0) {
		usageErr("%s temp-file [num-1kb-blocks]\n", av[0]);
	}

	int num_blks = (ac > 2) ? getInt(av[2], GN_GT_0, "num-1kb-blocks") :
				  100000;

	int fd = open(av[1], O_WRONLY | O_CREAT | O_EXCL, S_IROTH | S_IWUSR);
	if (fd == -1) {
		errExit("open");
	}

	if (unlink(av[1]) == -1) {
		errExit("unlink");
	}

	for (int j = 0; j < num_blks; j += 1) {
		if (write(fd, buf, BUF_SIZE) != BUF_SIZE) {
			fatal("partial/failed write");
		}
	}

	snprintf(shell_cmd, CMD_SIZE, "df -k `dirname %s`", av[1]);
	system(shell_cmd);

	// file is destroyed now
	if (close(fd) == -1) {
		errExit("close");
	}

	printf("********** Closed file descriptor\n");

	system(shell_cmd);
	exit(EXIT_SUCCESS);
}