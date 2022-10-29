#include "getcwd.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

// Return a heap-allocated string on success. NULL on error or cases where there
// are no more entries. If an error occurred, then `errno` is set to indicate the
// error.
static char *next_entry(void)
{
	struct stat cur_dir_metadata;
	struct stat parent_metadata;
	if (stat(".", &cur_dir_metadata) == -1) {
		return NULL;
	}
	if (stat("..", &parent_metadata) == -1) {
		return NULL;
	}

	// reach root
	if (cur_dir_metadata.st_dev == parent_metadata.st_dev &&
	    cur_dir_metadata.st_ino == parent_metadata.st_ino) {
		return NULL;
	}

	// cd to the parent dir
	if (chdir("..") == -1) {
		return NULL;
	}

	DIR *dir_p = NULL;
	struct dirent *dirent_p = NULL;
	if ((dir_p = opendir(".")) == NULL) {
		return NULL;
	}

	while ((dirent_p = readdir(dir_p)) != NULL) {
		struct stat entry_metadata;
		if (stat(dirent_p->d_name, &entry_metadata) == -1) {
			return NULL;
		}

		if (entry_metadata.st_dev == cur_dir_metadata.st_dev &&
		    entry_metadata.st_ino == cur_dir_metadata.st_ino) {
			char *ret = malloc(256);
			memset(ret, 0, 256);
			strncpy(ret, dirent_p->d_name,
				strlen(dirent_p->d_name) + 1);
			closedir(dir_p);
			return ret;
		}
	}

	closedir(dir_p);
	// unreachable
	return NULL;
}

// The return value is heap-allocated, use `free(3)` to deallocate it.
//
// On error, NULL will be returned and `errno` is set to indicate the error.
char *my_getcwd(void)
{
	int cwd_fd = open(".", O_RDONLY);
	if (cwd_fd == -1) {
		return NULL;
	}
	char *res = malloc(PATH_MAX);
	memset(res, 0, PATH_MAX);
	int len_of_filled_bytes = 0;

	char *entry = NULL;
	errno = 0; // clear errno
	while ((entry = next_entry()) != NULL) {
		int len_of_entry = (int)strlen(entry);
		// copy entry
		char* write_addr =
			res + (PATH_MAX - len_of_entry - len_of_filled_bytes);
		memcpy(write_addr, entry, strlen(entry));
		len_of_filled_bytes += len_of_entry;
		write_addr -= 1;

		// copy "/"
		memcpy(write_addr, "/", 1);
		len_of_filled_bytes += 1;

		free(entry);
	}
	// an error occurred
	if (errno != 0) {
		return NULL;
	}
	if (fchdir(cwd_fd) == -1) {
		return NULL;
	}

	// every thing is fine, ready to return.
	memmove(res, res + PATH_MAX - len_of_filled_bytes, len_of_filled_bytes);
	res[len_of_filled_bytes] = '\0';
	return res;
}