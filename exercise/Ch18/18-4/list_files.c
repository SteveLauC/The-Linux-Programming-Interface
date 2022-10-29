#include <assert.h>
#include <dirent.h>
#include "../../../lib/tlpi_hdr.h"

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
	Boolean is_current = (strcmp(".", dirpath) == 0);

	dir_p = opendir(dirpath);
	if (dir_p == NULL) {
		errMsg("failed on %s", dirpath);
		return;
	}

	for (;;) {
		struct dirent buf;
		struct dirent *result = NULL;
		int ret = readdir_r(dir_p, &buf, &result);

		// no more entries
		if (result == NULL) {
			break;
		}
		// an error occurred.
		if (ret > 0) {
			errMsg("readdir_r");
		}

		// skip "." and ".."
		if (strcmp(".", buf.d_name) == 0 ||
		    strcmp("..", buf.d_name) ==0 {
			continue;
		}

		if (!is_current) {
			printf("%s", dirpath);
		}
		printf("%s\n", buf.d_name);
	}

	if (closedir(dir_p) == -1) {
		errMsg("closedir");
	}
}
