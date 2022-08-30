#include <pwd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct passwd *my_getpwnam(const char *);

int main(void)
{
	printf("%s\n", my_getpwnam("steve")->pw_name);

	return EXIT_SUCCESS;
}

struct passwd *my_getpwnam(const char *name)
{
	struct passwd *p = getpwent();
	while (p != NULL) {
		// find it
		if (strncmp(p->pw_name, name, strlen(name)) == 0) {
			endpwent();
			return p;
		}
		// keep searching
		p = getpwent();
	}

	// reach NULL: on error or no more entries left
	endpwent();
	return p;
}
