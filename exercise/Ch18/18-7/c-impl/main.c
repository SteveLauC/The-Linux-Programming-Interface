#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>

int dir = 0;
int reg = 0;
int symlink = 0;
int fifo = 0;
int sock = 0;
int char_dev = 0;
int blk_dev = 0;

int count(const char *, const struct stat *, int, struct FTW *);

static void usage(const char *prog_name)
{
	fprintf(stderr, "Usage: %s directory", prog_name);
	exit(EXIT_FAILURE);
}

int count(const char *fpath, const struct stat *sb, int typeflag,
	  struct FTW *ftwbuf)
{
	if (typeflag == FTW_NS) {
		return -1;
	}

	switch (sb->st_mode & S_IFMT) {
	case S_IFREG:
		reg += 1;
		break;
	case S_IFDIR:
		dir += 1;
		break;
	case S_IFCHR:
		char_dev += 1;
		break;
	case S_IFBLK:
		blk_dev += 1;
		break;
	case S_IFLNK:
		symlink += 1;
		break;
	case S_IFIFO:
		fifo += 1;
		break;
	case S_IFSOCK:
		sock += 1;
		break;
	}
	return 0;
}

int main(int ac, char *av[])
{
	if (ac != 2) {
		usage(av[0]);
	}

	const char *target_dir = av[1];
	struct stat buf;
	if (stat(target_dir, &buf) == -1) {
		fprintf(stderr, "stat %s failed\n", target_dir);
		exit(EXIT_FAILURE);
	}
	if (!S_ISDIR(buf.st_mode)) {
		fprintf(stderr, "%s is not a directory\n", target_dir);
		usage(av[0]);
	}

	nftw(target_dir, count, 20, 0);
	int total = dir + reg + fifo + symlink + sock + char_dev + blk_dev;

	printf("directory: %d (%.3f)\n"
	       "regular file: %d (%.3f)\n"
	       "FIFO: %d (%.3f)\n"
	       "symlink: %d (%.3f)\n"
	       "socket: %d (%.3f)\n"
	       "char device: %d (%.3f)\n"
	       "block device: %d (%.3f)\n",
	       dir, (double)dir / total, reg, (double)reg / total, fifo,
	       (double)fifo / total, symlink, (double)symlink / total, sock,
	       (double)sock / total, char_dev, (double)char_dev / total,
	       blk_dev, (double)blk_dev / total);

	exit(EXIT_SUCCESS);
}
