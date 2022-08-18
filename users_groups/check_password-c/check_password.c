/*
 * compile: $ gcc check_password.c getpass.c ../../lib/error_functions.c -lcrypt -o check_password
*/

#include "../../lib/tlpi_hdr.h"
#include "getpass.h"
#include <pwd.h>
#include <shadow.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	long lnmax = sysconf(_SC_LOGIN_NAME_MAX);
	if (lnmax == -1) {
		lnmax = 256;
	}

	char username[lnmax];
	printf("Username: ");
	fflush(stdout);
	if (fgets(username, lnmax, stdin) == NULL) {
		exit(EXIT_FAILURE);
	}

	// remove trailing '\n'
	unsigned long len = strlen(username);
	username[len - 1] = '\0';

	struct passwd *pwd = getpwnam(username);
	if (pwd == NULL) {
		fatal("couldn't get password record");
	}

	struct spwd *spwd = getspnam(username);
	if (spwd == NULL && errno == EACCES) {
		fatal("no permission to read shadow file");
	}

	if (spwd != NULL) {
		pwd->pw_passwd = spwd->sp_pwdp;
	}

	char *password = my_getpass("Password: ");
	char *encrypted = crypt(password, pwd->pw_passwd);
	for (char *p = password; *p != '\0';) {
		*p++ = '\0';
	}

	if (encrypted == NULL) {
		errExit("crypt");
	}

	if (strcmp(encrypted, pwd->pw_passwd) != 0) {
		printf("Incorrect password\n");
		exit(EXIT_FAILURE);
	}

	printf("Successfully authenticated: UID=%ld\n", (long)pwd->pw_uid);
	exit(EXIT_SUCCESS);
}
