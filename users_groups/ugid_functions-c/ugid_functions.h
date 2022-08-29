#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


char *userNameFromId(uid_t uid);
int64_t userIdFromName(const char *name);
char *groupNameFromId(gid_t gid);
int64_t groupIdFromName(const char *name);
