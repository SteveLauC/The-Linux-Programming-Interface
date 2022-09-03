#include "../lib/tlpi_hdr.h"
#include <time.h>
#include <sys/time.h>

#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 * 60)

int main(void)
{
	const time_t t =
		time(NULL); // no error hanlding as it does not need this.
	printf("Seconds since the Epoch (1 Jan 1970): %ld", (long)t);
	printf(" (about %6.3f years)\n", t / SECONDS_IN_TROPICAL_YEAR);

	struct timeval tv;
	if (gettimeofday(&tv, NULL) == -1) {
		errExit("gettimeofday");
	}
	printf(" gettimeofday() returned %ld seconds, %ld microsecs\n",
	       (long)tv.tv_sec, (long)tv.tv_usec);

	struct tm gm;
	if (NULL == gmtime_r(&t, &gm)) {
		errExit("gmtime_r");
	}
	printf("Broken-down by gmtime():\n");
	printf("   year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", gm.tm_year,
	       gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
	printf("wday=%d yday=%d isdst=%d\n", gm.tm_wday, gm.tm_yday,
	       gm.tm_isdst);

	struct tm loc;
	if (NULL == localtime_r(&t, &loc)) {
		errExit("localtime");
	}
	printf("Broken-down by localtime():\n");
	printf("   year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", loc.tm_year,
	       loc.tm_mon, loc.tm_mday, loc.tm_hour, loc.tm_min, loc.tm_sec);
	printf("wday=%d yday=%d isdst=%d\n", loc.tm_wday, loc.tm_yday,
	       loc.tm_isdst);

	char buf[BUFSIZ];

	if (NULL == asctime_r(&gm, buf)) {
		errExit("asctime");
	}
	printf("asctime() formats the gmtime() value as %s", buf);
	if (NULL == ctime_r(&t, buf)) {
		errExit("ctime");
	}
	printf("ctime() formats the time() value as %s", buf);

	printf("mktime() of gmtime() value: %ld secs\n", (long)mktime(&gm));
	printf("mktime() of localtime() value: %ld secs\n", (long)mktime(&loc));

	exit(EXIT_SUCCESS);
}
