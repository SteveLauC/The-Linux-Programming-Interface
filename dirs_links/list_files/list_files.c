#include <assert.h>
#include <dirent.h>
#include "../../lib/tlpi_hdr.h"

static void list_files(const char *);

int main(int ac, char *av[])
{
	if (ac > 1 && strcmp(av[1], "--help") == 0) {
		usageErr("%s [dir...]\n", av[0]);
	}

	if (ac == 1) {
		list_files(".");
	} else {
		for (av++; *av; av++) {
			list_files(*av);
		}
	}

	exit(EXIT_SUCCESS);
}

static void list_files(const char *dirpath)
{
	assert(dirpath != NULL);

	DIR *dir_p = NULL;
	struct dirent *dirent_p = NULL;
	Boolean is_current = (strncmp(dirpath, ".", strlen(".")) == 0);

	dir_p = opendir(dirpath);
	if (dir_p == NULL) {
		errMsg("failed on %s", dirpath);
		return;
	}

	for (;;) {
		// To distinguish error form end-of-directory
		errno = 0;
		dirent_p = readdir(dir_p);
		if (dirent_p == NULL) {
			break;
		}

		if (strncmp(dirent_p->d_name, ".", strlen(".")) == 0 ||
		    strncmp(dirent_p->d_name, "..", strlen("..")) == 0) {
			continue; // skip "." and ".."
		}

		if (!is_current) {
			printf("%s", dirpath);
		}
		printf("%s\n", dirent_p->d_name);
	}

	if (errno != 0) {
		errExit("readdir");
	}

	if (closedir(dir_p) == -1) {
		errMsg("closedir");
	}
}
