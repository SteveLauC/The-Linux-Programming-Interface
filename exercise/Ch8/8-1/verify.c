#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("%s %s\n", getpwnam("steve")->pw_name,
	       getpwnam("root")->pw_name);
	printf("%d %d\n", getpwnam("steve")->pw_uid, getpwnam("root")->pw_uid);

	return EXIT_SUCCESS;
}
