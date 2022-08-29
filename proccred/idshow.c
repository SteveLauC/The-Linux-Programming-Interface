/*
 * compile: gcc idshow.c ../users_groups/ugid_functions-c/ugid_functions.h -o idshow
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <sys/fsuid.h>
#include "../users_groups/ugid_functions-c/ugid_functions.h"
#include "../lib/tlpi_hdr.h"

#define SG_SIZE (NGROUPS_MAX + 1)

int main(void)
{
	uid_t ruid, euid, suid, fsuid;
	gid_t rgid, egid, sgid, fsgid;
	gid_t suppGroups[SG_SIZE];
	int32_t numGroups, j;
	char *p = NULL;

	if (getresuid(&ruid, &euid, &suid) == -1) {
		errExit("getresuid");
	}

	if (getresgid(&rgid, &egid, &sgid) == -1) {
		errExit("getresgid");
	}

	// Attempts to change fs-IDs to values other than current RID, EID, SID and fsID
	// by a unpriviledged process will fail
	fsuid = setfsuid(0);
	fsgid = setfsgid(0);

	printf("UID: ");
	p = userNameFromId(ruid);
	printf("real = %s (%ld);", (p == NULL) ? "???" : p, (long)ruid);
	p = userNameFromId(euid);
	printf("eff = %s (%ld);", (p == NULL) ? "???" : p, (long)euid);
	p = userNameFromId(suid);
	printf("saved = %s (%ld);", (p == NULL) ? "???" : p, (long)suid);
	p = userNameFromId(fsuid);
	printf("fs = %s (%ld);", (p == NULL) ? "???" : p, (long)fsuid);
	printf("\n");

	printf("GID: ");
	p = groupNameFromId(rgid);
	printf("real = %s (%ld);", (p == NULL) ? "???" : p, (long)rgid);
	p = groupNameFromId(egid);
	printf("real = %s (%ld);", (p == NULL) ? "???" : p, (long)egid);
	p = groupNameFromId(sgid);
	printf("real = %s (%ld);", (p == NULL) ? "???" : p, (long)sgid);
	p = groupNameFromId(fsgid);
	printf("fs = %s (%ld);", (p == NULL) ? "???" : p, (long)fsgid);
	printf("\n");

	numGroups = getgroups(SG_SIZE, suppGroups);
	if (numGroups == -1) {
		errExit("getgroups");
	}

	printf("%d supplementary groups: ", numGroups);
	for (j = 0; j < numGroups; j += 1) {
		p = groupNameFromId(suppGroups[j]);
		printf("%s (%ld);", (p == NULL) ? "???" : p,
		       (long)suppGroups[j]);
	}
	printf("\n");

	exit(EXIT_SUCCESS);
}
