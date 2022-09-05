#include <time.h>
#include <sys/times.h>
#include "../lib/tlpi_hdr.h"

static void display_process_times(const char *);

int main(int ac, char *av[])
{
	int num_calls = (ac > 1) ? getInt(av[1], GN_GT_0, "num-calls") :
				   100000000;
	printf("CLOCK_PER_SEC=%ld sysconf(_SC_CLK_TCK)=%ld\n\n", CLOCKS_PER_SEC,
	       sysconf(_SC_CLK_TCK));

	display_process_times("At program start:\n");

	for (int i = 0; i < num_calls; i += 1) {
		getppid();
	}

	display_process_times("After getppid() loop:\n");
	exit(EXIT_SUCCESS);
}

// display `msg` and `process times`
static void display_process_times(const char *msg)
{
	struct tms buf;
	clock_t clock_time;
	static long clock_ticks = 0;
	if (msg != NULL) {
		printf("%s", msg);
	}

	// obtain clock_ticks on first call
	if (clock_ticks == 0) {
		clock_ticks = sysconf(_SC_CLK_TCK);
		if (clock_ticks == -1) {
			errExit("sysconf(_SC_CLK_TCK)");
		}
	}

	// call clock(3)
	clock_time = clock();
	if (clock_time == -1) {
		errExit("clock(3)");
	}
	printf("	clock() returns: %ld clocks-per-sec (%.2f secs)\n",
	       (long)clock_time, (double)clock_time / CLOCKS_PER_SEC);

	// call times(2)
	if (times(&buf) == -1) {
		errExit("times(2)");
	}
	printf("	times() yields: user CPU=%.2fsecs; sys CPU: %.2fsecs\n",
	       (double)buf.tms_utime / clock_ticks,
	       (double)buf.tms_stime / clock_ticks);
}
