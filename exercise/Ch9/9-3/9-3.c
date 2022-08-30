#define _GNU_SOURCE
#include <linux/limits.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <grp.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>

#define BUFLEN 4096

int my_initgroups(const char *, gid_t);

int main(void)
{
	assert(my_initgroups("steve", 999) == 0);

	int num_groups = getgroups(0, NULL);
	gid_t groups[num_groups];
	assert(-1 != getgroups(num_groups, groups));
	printf("Supplementary GIDs of current process: ");
	for (int idx = 0; idx < num_groups; idx += 1) {
		printf("%d ", groups[idx]);
	}
	printf("\n");
}

int my_initgroups(const char *user, gid_t group)
{
	struct passwd pw_buf;
	struct passwd *result = NULL;
	size_t bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == (size_t)-1) {
		bufsize = 16384;
	}
	char pw_str_buf[bufsize];

	// group array used in `setgroup`
	gid_t groups[NGROUPS_MAX + 1];
	int64_t num_groups = 0;
	// add `group` to `groups`
	groups[num_groups++] = group;

	int getpwnam_res =
		getpwnam_r(user, &pw_buf, pw_str_buf, bufsize, &result);

	// An error occurred during the execution of `getpwnam_r`
	if (getpwnam_res != 0 && result == NULL) {
		errno = getpwnam_res;
		return -1;
	}

	// `user` not found. The man page for `initgroup` does not mention
	// this case, I wrote a program to test it finding it will just ignore
	// this.
	// if (getpwnam_res == 0 && result == NULL) {}

	// `user` was found
	if (getpwnam_res == 0 && result != NULL) {
		// add the frist group to `groups`
		groups[num_groups++] = pw_buf.pw_gid;

		setgrent();
		struct group gr_buf;
		char gr_str_buf[BUFLEN];
		// sequentially scan `/etc/group` to find the remaining groups
		while (1) {
			struct group *result = NULL;
			int getgrent_r_res = getgrent_r(&gr_buf, gr_str_buf,
							BUFLEN, &result);
			if (getgrent_r_res == 0) {
				char **p = gr_buf.gr_mem;
				while (*p != NULL) {
					if (strstr(*p, user) != NULL) {
						groups[num_groups++] =
							gr_buf.gr_gid;
						break;
					}

					p += 1;
				}
			}
			// An error occurred
			if (getgrent_r_res == ERANGE) {
				errno = ERANGE;
				return -1;
			}
			if (getgrent_r_res == ENOENT) {
				// all the entries are scanned, break the while loop
				endgrent();
				break;
			}
		}
	}

	// all the groups are found, call `setgroups()`
	return setgroups(num_groups, groups);
}
