#include "my_access.h"

inline static int executable(mode_t);
static int matches_group(gid_t, gid_t, const gid_t *, int);

int my_eaccess(const char *path, int amode)
{
	struct stat buf;
	int ret_of_stat = stat(path, &buf);
	// my_eaccess relies on `stat(2)`, if `stat(2)` failed, we can directly
	// return.
	if (ret_of_stat == -1) {
		return -1;
	}

	// `stat(2)` succeeds, which means `path` exists, then we can ignore `F_OK`

	uid_t e_uid = geteuid();
	gid_t e_gid = getegid();
	int supp_groups_buf_size = getgroups(0, NULL);
	gid_t supp_groups[supp_groups_buf_size];
	if (-1 == getgroups(supp_groups_buf_size, supp_groups)) {
		return -1;
	}

	// if privileged, `r` and `w` permissions are granted
	// and if this file has `x` permission enabled on any categories of users, `x` is also granted
	if (0 == e_uid) {
		if ((amode & X_OK) && !executable(buf.st_mode)) {
			return -1;
		}

		return 0;
	} else if (e_uid == buf.st_uid) {
		if (amode & R_OK && (buf.st_mode & S_IRUSR) == 0) {
			return -1;
		}

		if ((amode & W_OK) && (buf.st_mode & S_IWUSR) == 0) {
			return -1;
		}

		if ((amode & X_OK) && (buf.st_mode & S_IXUSR) == 0) {
			return -1;
		}
		return 0;
	} else if (matches_group(e_gid, buf.st_gid, supp_groups,
				 supp_groups_buf_size)) {
		if (amode & R_OK && (buf.st_mode & S_IRGRP) == 0) {
			return -1;
		}

		if ((amode & W_OK) && (buf.st_mode & S_IWGRP) == 0) {
			return -1;
		}

		if ((amode & X_OK) && (buf.st_mode & S_IXGRP) == 0) {
			return -1;
		}
		return 0;
	} else {
		if (amode & R_OK && (buf.st_mode & S_IROTH) == 0) {
			return -1;
		}

		if ((amode & W_OK) && (buf.st_mode & S_IWOTH) == 0) {
			return -1;
		}

		if ((amode & X_OK) && (buf.st_mode & S_IXOTH) == 0) {
			return -1;
		}
		return 0;
	}
}

// return 1 if mode have `x` set. Otherwise, return 0
inline static int executable(mode_t mode)
{
	return mode & S_IXUSR || mode & S_IXGRP || mode & S_IXOTH;
}

// return 1 if `e_gid` matches `group` or any of `supp_groups`. Otherwise, return 0.
static int matches_group(gid_t e_gid, gid_t group, const gid_t *supp_groups,
			 int supp_groups_len)
{
	if (e_gid == group) {
		return 1;
	}
	for (int idx = 0; idx < supp_groups_len; idx += 1) {
		if (e_gid == supp_groups[idx]) {
			return 1;
		}
	}

	return 0;
}
