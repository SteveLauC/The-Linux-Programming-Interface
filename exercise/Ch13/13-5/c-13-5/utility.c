#include "utility.h"
#include <assert.h>

// Return the num of occurrences of `c` in `string`.
//
// `string` must be NUL-terminated.
int num_occurrences(const char *string, char c)
{
	assert(string != NULL);
	int count = 0;
	for (const char *p = string; *p != '\0'; p += 1) {
		if (*p == c) {
			count += 1;
		}
	}

	return count;
}

// argument initialization
void arg_init(char **filename, int *n_p, int ac, char **av)
{
	if (ac < 2) {
		fprintf(stderr, "Usage: %s [-n] FILE", av[0]);
		exit(EXIT_FAILURE);
	}

	if (ac == 2) {
		*filename = av[1];
	} else {
		*filename = av[2];
		errno = 0;
		*n_p = (int)strtol((av[1]) + 1, NULL, 10);
		if (errno != 0) {
			fprintf(stderr, "invalid -n option");
			exit(EXIT_FAILURE);
		}
	}

	assert(*filename != NULL);
}

// find the starting location and length of nth line, n starts from 1.
//
// `string` must be NUL-terminated
int nth_line(const char *string, int n, char **location, long *length)
{
	assert(string != NULL);

	const char *start_idx = string;
	const char *res = NULL;

	for (int i = 1; i < n; i += 1) {
		res = strchrnul(start_idx, '\n');
		if (*res == '\0') {
			return -1;
		}
		start_idx = res + 1;
	}
	*location = (char *)start_idx;
	char *next_newline_or_nul = strchrnul(start_idx, '\n');
	*length = (next_newline_or_nul - start_idx);
	return 0;
}

// If the buffer is someting like ['l', 'i', 'n', 'e', '1', '\n', 'l', 'i']
// After trim, it will be ['l', 'i', 'n', 'e', '1', '\n'], and the file will
// be seeked backwards.
//
// NOTE: `string` has to contain at least 1 `\n`.
void trim_buf(char *string, int fd)
{
	assert(string != NULL);

	size_t len = strlen(string);
	if (*(string + len - 1) != '\n') {
		char *last_newline = strrchr(string, '\n');
		assert(last_newline != NULL);

		assert(lseek(fd, last_newline - (string + len) + 1, SEEK_CUR) !=
		       -1);
		*(last_newline + 1) = '\0';
	}
}
