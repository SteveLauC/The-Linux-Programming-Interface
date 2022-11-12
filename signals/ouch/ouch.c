#include <signal.h>
#include "../../lib/tlpi_hdr.h"

static void sig_handler(int signum)
{
	printf("Ouch!\n");
}

int main(void)
{
	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		errExit("signal");
	}

	for (int j = 0; ; j++) {
		printf("%d\n", j);
		sleep(3);
	}
}
