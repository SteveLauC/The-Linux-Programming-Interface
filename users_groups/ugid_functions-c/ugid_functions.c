#include "ugid_functions.h"

/*
 * Return name corresponding to `uid`, or NULL on error
*/
char *userNameFromId(uid_t uid)
{
	struct passwd *pwd = getpwuid(uid);

	return (pwd == NULL) ? NULL : pwd->pw_name;
}

/*
 * Return UID corresponding to `name`, or -1 on error
*/
uid_t userIdFromName(const char *name)
{
	// empty string
	if (name == NULL || *name == '\0') {
		return -1;
	}

	// name is numeric string
	char *endptr;
	uid_t u = strtol(name, &endptr, 10);
	if (*endptr == '\0') {
		return u;
	}

	struct passwd *pwd = getpwnam(name);
	return (pwd == NULL) ? -1 : pwd->pw_uid;
}

/*
 * Return name corresponding to `gid`, or NULL on error
*/
char *groupNameFromId(gid_t gid)
{
	struct group *grp = getgrgid(gid);
	return (grp == NULL) ? NULL : grp->gr_name;
}

/*
 * Return GID corresponding to `name`, or -1 on error
*/
gid_t groupIdFromName(const char *name)
{
	// empty string
	if (name == NULL || *name == '\0') {
		return -1;
	}

	// name is numeric string
	char *endptr;
	uid_t u = strtol(name, &endptr, 10);
	if (*endptr == '\0') {
		return u;
	}

	struct group *grp = getgrnam(name);
	return (grp == NULL) ? -1 : grp->gr_gid;
}
