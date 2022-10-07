#include <fcntl.h>
#include "../../../users_groups/ugid_functions-c/ugid_functions.h"
#include "../../../lib/tlpi_hdr.h"

int main(int ac, char *av[])
{
	uid_t uid;
	gid_t gid;
	int j;
	Boolean err_fnd = FALSE;

	if (ac < 3 || strcmp(av[1], "--help") == 0) {
		usageErr("%s owner group [file...]"
			 "	owner or group can be '-', "
			 "meaning leave unchanged\n",
			 av[0]);
	}

	if (strcmp(av[1], "-") == 0) {
		uid = -1;
	} else {
		uid = userIdFromName(av[1]);
		if (uid == -1) {
			fatal("No such user (%s)", av[1]);
		}
	}

	if (strcmp(av[2], "-") == 0) {
		gid = -1;
	} else {
		gid = groupIdFromName(av[2]);
		if (gid == -1) {
			fatal("No such group (%s)", av[2]);
		}
	}

	err_fnd = FALSE;

	for (j = 3; j < ac; j += 1) {
		if (fchownat(AT_FDCWD, av[j], uid, gid, 0) == -1) {
			errMsg("chown: %s", av[j]);
			err_fnd = TRUE;
		}
	}

	exit(err_fnd ? EXIT_FAILURE : EXIT_SUCCESS);
}