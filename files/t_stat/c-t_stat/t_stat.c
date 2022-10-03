/*
 * usage: ./t_stat [-l] file
 */
#include <stdio.h>
#include <sys/sysmacros.h>
#include <sys/stat.h>
#include <time.h>
#include "file_perms.h"
#include "../../../lib/tlpi_hdr.h"

static void display_stat_info(const struct stat *);

int main(int ac, char *av[])
{
	if (ac <= 1 || (ac == 3 && strncmp(av[1], "-l", strlen("-l")) != 0) ||
	    ac > 3) {
		usageErr("%s [-l] file\n", av[1]);
	}
	struct stat buf;
	int stat_link = 0;
	int fname_idx;

	stat_link = (ac > 1) && strncmp(av[1], "-l", strlen("-l")) == 0;
	fname_idx = stat_link ? 2 : 1;

	if (stat_link) {
		if (lstat(av[fname_idx], &buf) == -1) {
			errExit("lstat");
		}
	} else {
		if (stat(av[fname_idx], &buf) == -1) {
			errExit("stat");
		}
	}

	display_stat_info(&buf);
	exit(EXIT_SUCCESS);
}

static void display_stat_info(const struct stat *sb)
{
	printf("File type: ");
	switch (sb->st_mode & S_IFMT) {
	case S_IFDIR:
		printf("directory\n");
		break;
	case S_IFCHR:
		printf("character device\n");
		break;
	case S_IFBLK:
		printf("block device\n");
		break;
	case S_IFREG:
		printf("regular file\n");
		break;
	case S_IFIFO:
		printf("FIFO or pipe\n");
		break;
	case S_IFLNK:
		printf("symbolic (soft) link\n");
		break;
	case S_IFSOCK:
		printf("socket\n");
		break;
	default:
		printf("unknown file type?\n");
		break;
	}

	printf("Device containing i-node: major=%ld minor=%ld\n",
	       (long)major(sb->st_dev), (long)minor(sb->st_dev));
	printf("I-node number: %ld\n", sb->st_ino);
	printf("Mode: %lo (%s)\n", (unsigned long)sb->st_mode,
	       filePermStr(sb->st_mode, 1));

	printf("Special bits set: %s%s%s\n", 
			(sb->st_mode & S_ISUID) ? "set-UID" : "", 
			(sb->st_mode & S_ISGID) ? "set-GID" : "",
			(sb->st_mode & S_ISVTX) ? "sticky" : ""
	      );

	printf("Number of (hard) links: %ld\n", (long)sb->st_nlink);
	printf("Ownership: UID=%ld GID=%ld\n", (long)sb->st_uid,
	       (long)sb->st_gid);
	if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode)) {
		printf("Device number (st_rdev): major=%ld minor=%ld\n",
		       (long)major(sb->st_rdev), (long)minor(sb->st_rdev));
	}
	printf("File size: %lld bytes\n", (long long)sb->st_size);
	printf("Optimal I/O block size: %ld bytes\n", (long)sb->st_blksize);
	printf("512B blocks allocated: %lld\n", (long long)sb->st_blocks);
	char time_str_buf[BUFSIZ];
	printf("Last file access: %s",
	       ctime_r(&sb->st_atim.tv_sec, time_str_buf));
	printf("Last file modification: %s",
	       ctime_r(&sb->st_mtim.tv_sec, time_str_buf));
	printf("Last status change: %s",
	       ctime_r(&sb->st_ctim.tv_sec, time_str_buf));
}
