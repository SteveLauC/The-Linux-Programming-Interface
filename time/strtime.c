/*
$ gccs strtime.c ../lib/error_functions.c
$ ./a.out "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y"
calendar time (secs since epoch): 1296567586
strftime() yields: 09:39:46PM 01 Feb 2011
*/

#define _XOPEN_SOURCE
#include <time.h>
#include <locale.h>
#include "../lib/tlpi_hdr.h"

#define SBUF_SIZE 1000

int main(int ac, char *av[])
{
	if (ac < 3 || ac > 4) {
		usageErr("%s input-date-time in-format [out-format]\n", av[0]);
	}

	// use lcoale settings in conversions
	if (setlocale(LC_ALL, "") == NULL) {
		errExit("setlocal");
	}

	struct tm tm_buf;
	char str_buf[SBUF_SIZE];

	// initialize `tm_buf`
	memset(&tm_buf, 0, sizeof(tm_buf));

	if (strptime(av[1], av[2], &tm_buf) == NULL) {
		fatal("strptime");
	}
	// tm_isdist is not set, tell `mktime(3)` to determine this itself.
	tm_buf.tm_isdst = -1;

	printf("calendar time (secs since epoch): %ld\n",
	       (long)mktime(&tm_buf));

	char *out_format = (ac == 4) ? av[3] : av[2];
	// A return of `0` does not necessarily indicate an error
	if (strftime(str_buf, SBUF_SIZE, out_format, &tm_buf) == 0) {
		fatal("strftime returned 0");
	}
	printf("strftime() yields: %s\n", str_buf);

	exit(EXIT_SUCCESS);
}
