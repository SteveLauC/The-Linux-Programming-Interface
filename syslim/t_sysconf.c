#include "../lib/tlpi_hdr.h"

static void sysconf_print(const char *, int);

int main(void)
{
	sysconf_print("_SC_ARG_MAX", _SC_ARG_MAX);
	sysconf_print("_SC_LOGIN_NAME_MAX", _SC_LOGIN_NAME_MAX);
	sysconf_print("_SC_OPEN_MAX", _SC_OPEN_MAX);
	sysconf_print("_SC_NGROUPS_MAX", _SC_NGROUPS_MAX);
	sysconf_print("_SC_PAGE_SIZE", _SC_PAGE_SIZE);
	sysconf_print("_SC_RTSIG_MAX", _SC_RTSIG_MAX);

	exit(EXIT_SUCCESS);
}

// print `msg` plus sysconf() value for `name`
static void sysconf_print(const char *msg, int name)
{
	errno = 0;
	long lim = sysconf(name);

	if (lim != -1) {
		printf("%s %ld\n", msg, lim);
	} else {
		if (errno == 0) {
			printf("%s (indeterminate)\n", msg);
		} else {
			errExit("sysconf %s", msg);
		}
	}
}
