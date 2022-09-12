#include "../../../../lib/tlpi_hdr.h"
#include <dirent.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int main(int ac, char *av[])
{
	if (ac != 2) {
		usageErr("Usage: 12-3 <PATH>");
	}

	const char *path = av[1];
	DIR *dir_ptr = opendir("/proc");
	if (dir_ptr == NULL) {
		errExit("can not open /proc");
	}
	assert(dir_ptr != NULL);
	struct dirent *dirent_ptr = NULL;
	while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
		if (!isdigit(dirent_ptr->d_name[0])) {
			continue;
		}
		char fd_path[300] = { 0 }; // /proc/PID/fd
		char status_path[300] = { 0 }; // /proc/PID/status
		sprintf(fd_path, "/proc/%s/fd", dirent_ptr->d_name);
		sprintf(status_path, "/proc/%s/status", dirent_ptr->d_name);

		DIR *fd_dir_ptr = opendir(fd_path);
		if (fd_dir_ptr == NULL) {
			// you may don' t have sufficient permission to open this directory
			continue;
		}
		struct dirent *fd_dirent_ptr = NULL;
		while ((fd_dirent_ptr = readdir(fd_dir_ptr)) != NULL) {
			if (!isdigit(fd_dirent_ptr->d_name[0])) {
				continue;
			}
			// construct the full path of this fd file
			char full_path_of_opened_file[300] = { '\0' };
			strncpy(full_path_of_opened_file, fd_path,
				strlen(fd_path) + 1);
			strncpy(full_path_of_opened_file +
					strlen(full_path_of_opened_file),
				"/", strlen("/"));
			strncpy(full_path_of_opened_file +
					strlen(full_path_of_opened_file),
				fd_dirent_ptr->d_name,
				strlen(fd_dirent_ptr->d_name) + 1);

			// fetch the file this soft link pointing to
			char soft_link_contents[256] = { 0 };
			assert(readlink(full_path_of_opened_file,
					soft_link_contents, 256) != -1);

			// this file satisfies the condition
			if (strncmp(soft_link_contents, path, strlen(path)) ==
			    0) {
				FILE *status_fp = fopen(status_path, "r");
				if (status_fp != NULL) {
					// read process name and print it out
					char name[50];
					fgets(name, 50, status_fp);
					printf("%s", name);
				}
				break;
			}
		}
		closedir(fd_dir_ptr);
	}

	closedir(dir_ptr);
}
