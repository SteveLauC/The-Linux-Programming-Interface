#include "../../lib/error_functions.h"
#include "getpass.h"
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

char *my_getpass(const char *prompt)
{
	// disable ECHO bit
	struct termios buf;
	if (0 != tcgetattr(0, &buf)) {
		errExit("tcgetattr");
	}
	buf.c_lflag &= ~ECHO;

	if (0 != tcsetattr(0, TCSANOW, &buf)) {
		errExit("tcsetattr");
	}

	// ignore the SIGINT signal
	signal(SIGINT, SIG_IGN);

	printf("%s", prompt);
	fflush(stdout);

	static char pw[BUFSIZ];
	fgets(pw, BUFSIZ, stdin);
	unsigned long len = strlen(pw);
	pw[len - 1] = '\0';

	// restore the default setting
	if (0 != tcgetattr(0, &buf)) {
		errExit("tcgetattr");
	}
	buf.c_lflag |= ECHO;

	if (0 != tcsetattr(0, TCSANOW, &buf)) {
		errExit("tcsetattr");
	}
	signal(SIGINT, SIG_DFL);

	return pw;
}
