#include <sys/stat.h>
#include <fcntl.h>
#include "../../../lib/tlpi_hdr.h"
#include "../../t_stat/c-t_stat/file_perms.h"

#define MYFILE "myfile"
#define MYDIR "mydir"

#define FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) // rw-rx---- 0o660
#define DIR_PERMS (S_IRWXU | S_IRWXG | S_IRWXO) // rwxrwxrwx 0o777
#define UMASK_SETTING (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH) // 0o033

int main(void)
{
	umask(UMASK_SETTING);

	int fd = open(MYFILE, O_RDWR | O_CREAT | O_EXCL, FILE_PERMS);
	if (fd == -1) {
		errExit("open-%s", MYFILE);
	}
	if (mkdir(MYDIR, DIR_PERMS) == -1) {
		errExit("mkdir-%s", MYDIR);
	}

	struct stat stat_buf;
	if (stat(MYFILE, &stat_buf) == -1) {
		errExit("stat-%s", MYFILE);
	}
	printf("Requested file perms: %s\n", filePermStr(FILE_PERMS, 0));
	printf("Process umask: %s\n", filePermStr(UMASK_SETTING, 0));
	printf("Actual file perms: %s\n", filePermStr(stat_buf.st_mode, 0));

	if (stat(MYDIR, &stat_buf) == -1) {
		errExit("stat-%s", MYDIR);
	}

	printf("Requested dir perms: %s\n", filePermStr(DIR_PERMS, 0));
	printf("Process umask: %s\n", filePermStr(UMASK_SETTING, 0));
	printf("Actual dir perms: %s\n", filePermStr(stat_buf.st_mode, 0));

	if (unlink(MYFILE) == -1) {
		errMsg("unlink-%s", MYFILE);
	}

	if (rmdir(MYDIR) == -1) {
		errMsg("rmdir-%s", MYDIR);
	}

	exit(EXIT_SUCCESS);
}