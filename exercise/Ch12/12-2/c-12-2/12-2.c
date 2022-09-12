#include "../../../../lib/tlpi_hdr.h"
#include <dirent.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

// how many processes we can have
#define MAX_PROC 500
// the maximum length of the name of a cmd
#define MAX_CMD_NAME_LEN 256

typedef struct {
	char cmd[MAX_CMD_NAME_LEN];
	pid_t pid;
	pid_t ppid;
	int32_t process_idx;
} process;

typedef struct {
	process array[MAX_PROC];
	int32_t num; // how many processes `array` has
} processes;

void fetch_processes_info(processes *);
void set_process_info(process *, const char *);
void preorder_traverse(const processes *, int32_t, int32_t);
void set_parent_process_index(processes *);
void handle_disappeared_process(processes *);

int main(void)
{
	processes *p = (processes *)malloc(sizeof(processes));
	// initialization
	p->num = 0;
	for (int32_t idx = 0; idx < MAX_PROC; idx += 1) {
		*(p->array[idx].cmd) = '\0';
		p->array[idx].pid = 0;
		p->array[idx].ppid = 0;
		p->array[idx].process_idx = -1;
	}

	fetch_processes_info(p);
	set_parent_process_index(p);
	handle_disappeared_process(p);
	preorder_traverse(p, 0, 0);
	free(p);
}

// seqneucely scan `/proc/PID/status`, and store all the processes info
// into `procs`
void fetch_processes_info(processes *procs)
{
	DIR *dir_ptr = opendir("/proc");
	if (dir_ptr == NULL) {
		errExit("opendir(3)");
	}

	struct dirent *dirent_ptr = NULL;
	while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
		// filter non-digit directories
		if (!isdigit(dirent_ptr->d_name[0])) {
			continue;
		}

		char path[256];
		sprintf(path, "/proc/%s/status", dirent_ptr->d_name);
		set_process_info(&procs->array[procs->num], path);

		procs->num += 1;
	}

	closedir(dir_ptr);
}

// read particular `/proc/PID/status` specified in `path` and store it's
// `Name/pid/ppid` in `proc`
//
// The lines we need:
// 	Name:	zsh
// 	Pid:	4733
// 	PPid:	3674
void set_process_info(process *proc, const char *path)
{
	FILE *fp = fopen(path, "r");
	if (fp == NULL && errno == EACCES) {
		fprintf(stderr, "%s file disappears", path);
		return;
	}

	char line[BUFSIZ];
	while (fgets(line, BUFSIZ, fp) != NULL) {
		if (strncmp("Name", line, strlen("Name")) == 0) {
			char cmd[MAX_CMD_NAME_LEN];
			sscanf(line, "Name:\t%s", cmd);
			// use `strlen(cmd)+1` to include the tailing NUL
			strncpy(proc->cmd, cmd, strlen(cmd) + 1);
		}
		if (strncmp("Pid", line, strlen("Pid")) == 0) {
			pid_t pid = -1;
			sscanf(line, "Pid:\t%d", &pid);
			proc->pid = pid;
		}
		if (strncmp("PPid", line, strlen("PPid")) == 0) {
			pid_t ppid = -1;
			sscanf(line, "PPid:\t%d", &ppid);
			proc->ppid = ppid;
			break;
		}
	}

	fclose(fp);
}

// traverse the processes array in preorder
void preorder_traverse(const processes *procs, const int32_t index,
		       const int32_t level)
{
	// prints `level` '\t'
	for (int32_t idx = 0; idx < level; idx += 1) {
		printf("\t");
	}
	printf("%d %s\n", procs->array[index].pid, procs->array[index].cmd);

	// then prints all the child processes information
	for (int32_t j = 0; j < procs->num; j += 1) {
		if (procs->array[j].process_idx == index) {
			preorder_traverse(procs, j, level + 1);
		}
	}
}

// set the index of the parent process
// If a process has no parent, then the `process_idx` field will remain `-1`
void set_parent_process_index(processes *p)
{
	for (int i = 0; i < p->num; i++) {
		for (int j = 0; j < p->num; j++) {
			if (p->array[i].ppid == p->array[j].pid) {
				p->array[i].process_idx = j;
				break;
			}
		}
	}
}

// Some processes may disappear when we try to read its `status` file
// This function will remove them from the processes array
void handle_disappeared_process(processes *procs)
{
	for (int32_t idx = 0; idx < procs->num; idx += 1) {
		// find it, swap `array[idx]` and `array[procs->num-1]`
		if (procs->array[idx].pid == 0) {
			if (idx != procs->num - 1) {
				procs->array[idx].pid =
					procs->array[procs->num - 1].pid;
				procs->array[idx].ppid =
					procs->array[procs->num - 1].ppid;
				procs->array[idx].process_idx =
					procs->array[procs->num - 1].process_idx;
				strncpy(procs->array[idx].cmd,
					procs->array[procs->num - 1].cmd,
					strlen(procs->array[procs->num - 1].cmd) +
						1);
			}
			procs->num -= 1;
		}
	}
}
