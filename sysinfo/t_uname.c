#include <stdio.h>
#include <stdlib.h>
#ifdef __linux
#define __USE_GNU
#endif

#include <sys/utsname.h>
#include "../lib/tlpi_hdr.h"

int main(void) {
	struct utsname uts;

	if (uname(&uts) == -1) {
		errExit("uname(2)");
	}

	printf("Node name:   %s\n", uts.nodename);
	printf("System name: %s\n", uts.sysname);
	printf("Release:     %s\n", uts.release);
	printf("Version:     %s\n", uts.version);
	printf("Machine:     %s\n", uts.machine);

#ifdef __USE_GNU
	printf("Domain name: %s\n", uts.domainname);
#endif
	exit(EXIT_SUCCESS);
}
