/*
 * a simple chattr(1)
 *
 * usage: ./chattr [mode] files...
 * where `mode` can be `+-=[acdeijstuADT]`
 */

#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <linux/fs.h>

struct map {
	char code;
	int attribute;
};

static struct map fs_attrs[] = {
	{ 'a', FS_APPEND_FL },	  { 'c', FS_COMPR_FL },
	{ 'd', FS_NODUMP_FL },	  { 'e', FS_EXTENT_FL },
	{ 'i', FS_IMMUTABLE_FL }, { 'j', FS_JOURNAL_DATA_FL },
	{ 's', FS_SECRM_FL },	  { 't', FS_NOTAIL_FL },
	{ 'u', FS_UNRM_FL },	  { 'A', FS_NOATIME_FL },
	{ 'D', FS_DIRSYNC_FL },	  { 'T', FS_TOPDIR_FL },
};

const char *AvailableFlags = "acdeijstuADT";

static int check_flag(const char *, const char *);
static int attribute_look_up(char);
static void update_attrs(const char *, int *);
static void action(int, char **);

int main(int ac, char *av[])
{
	if (ac < 3) {
		fprintf(stderr, "usage: %s [mode] files", av[0]);
		exit(EXIT_FAILURE);
	}
	if (check_flag((av[1] + 1), AvailableFlags) == 0) {
		fprintf(stderr, "invalid flags");
		exit(EXIT_FAILURE);
	}

	action(ac, av);
}

static int check_flag(const char *flags, const char *avail_flags)
{
	assert(flags != NULL);
	assert(avail_flags != NULL);

	int flags_len = (int)strlen(flags);
	int ava_flags_len = (int)strlen(avail_flags);

	for (int i = 0; i < flags_len; i += 1) {
		int is_flag_valid = 0;
		for (int j = 0; j < ava_flags_len; j += 1) {
			if (avail_flags[j] == flags[i]) {
				is_flag_valid = 1;
				continue;
			}
		}

		if (is_flag_valid == 0) {
			return 0;
		}
	}

	return 1;
}

static int attribute_look_up(char code)
{
	for (int i = 0; i < (int)(sizeof(fs_attrs) / sizeof(struct map));
	     i += 1) {
		if (fs_attrs[i].code == code) {
			return fs_attrs[i].attribute;
		}
	}

	fprintf(stderr, "unreachable");
	exit(EXIT_FAILURE);
}

static void update_attrs(const char *mode, int *attribute)
{
	int mode_len = (int)strlen(mode);
	if (*mode == '+') {
		for (int i = 1; i < mode_len; i += 1) {
			*attribute |= attribute_look_up(mode[i]);
		}
	} else if (*mode == '-') {
		for (int i = 1; i < mode_len; i += 1) {
			*attribute &= !attribute_look_up(mode[i]);
		}
	} else {
		int new_attr = 0;
		for (int i = 1; i < mode_len; i += 1) {
			new_attr |= attribute_look_up(mode[i]);
		}
		*attribute = new_attr;
	}
}

static void action(int ac, char *av[])
{
	const char *flags = av[1];
	for (int i = 2; i < ac; i += 1) {
		int fd = open(av[i], 0);
		if (fd == -1) {
			fprintf(stderr, "unabel to open %s\n", av[i]);
			close(fd);
			continue;
		}
		int attr = 0;
		if (-1 == ioctl(fd, FS_IOC_GETFLAGS, &attr)) {
			fprintf(stderr, "ERROR: Unable to get flags on %s\n",
				av[i]);
			perror(NULL);
			close(fd);
			continue;
		}
		update_attrs(flags, &attr);
		if (-1 == ioctl(fd, FS_IOC_SETFLAGS, &attr)) {
			fprintf(stderr, "ERROR: Unable to set flags on %s\n",
				av[i]);
			perror(NULL);
			close(fd);
			continue;
		}
	}
}