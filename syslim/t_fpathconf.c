#include "../lib/tlpi_hdr.h"

static void fpathconf_print(const char *, int, int);

int main(void)
{
	fpathconf_print("_PC_NAME_MAX", STDIN_FILENO, _PC_NAME_MAX);
	fpathconf_print("_PC_PATH_MAX", STDIN_FILENO, _PC_PATH_MAX);
	fpathconf_print("_PC_PIPE_BUF", STDIN_FILENO, _PC_PIPE_BUF);
	exit(EXIT_SUCCESS);
}

static void fpathconf_print(const char *msg, int fd, int name)
{
	errno = 0;
	long limit = fpathconf(fd, name);
	if (limit != -1) {
		printf("%s %ld\n", msg, limit);
	} else {
		if (errno == 0) {
			printf("%s (indeterminate)\n", msg);
		} else {
			errExit("fpathconf %s", msg);
		}
	}
}
