#include "../../../../lib/tlpi_hdr.h"
#include "../../../../users_groups/ugid_functions-c/ugid_functions.h"
#include <dirent.h>
#include <stdint.h>

void parse_status(char *, uid_t);
void dbg(char *arr, int32_t len);

int main(int ac, char *av[])
{
	if (ac != 2) {
		usageErr("12-1 <USER>");
	}
	uid_t uid = userIdFromName(av[1]);
	if (uid == (uid_t)-1) {
		errExit("userIdFromName");
	}

	DIR *dir_ptr = opendir("/proc");
	if (dir_ptr == NULL) {
		errExit("opendir");
	}

	struct dirent *dirent_ptr = NULL;
	while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
		if (!isdigit((int)dirent_ptr->d_name[0])) {
			continue;
		}
		char status_file_name[BUFSIZ];
		sprintf(status_file_name, "/proc/%s/status",
			dirent_ptr->d_name);
		parse_status(status_file_name, uid);
	}
	closedir(dir_ptr);
}

void parse_status(char *status_file, uid_t uid)
{
	FILE *fp = fopen(status_file, "r");
	if (fp == NULL) {
		errExit("fopen");
	}

	char line[BUFSIZ];
	char name_and_pid_buf[BUFSIZ];
	int32_t len = 0;

	while (NULL != fgets(line, BUFSIZ, fp)) {
		if (strncmp(line, "Name", strlen("Name")) == 0) {
			sscanf(line, "Name:\t%s", name_and_pid_buf + len);
			len += strlen(line) - strlen("Name:\t\n");
		}
		if (strncmp(line, "Pid", strlen("Pid")) == 0) {
			pid_t pid = 0;
			sscanf(line, "Pid:\t%d", &pid);
			sprintf(name_and_pid_buf + len, " %d", pid);
		}

		if (strncmp(line, "Uid", strlen("Uid")) == 0) {
			uid_t id = 0;
			sscanf(line, "Uid:\t%u", &id);
			if (uid == id) {
				printf("%s\n", name_and_pid_buf);
			}
			break;
		}
	}
	fclose(fp);
}
