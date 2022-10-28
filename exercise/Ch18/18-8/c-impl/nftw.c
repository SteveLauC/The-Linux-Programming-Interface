#include "nftw.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int32_t my_nftw(const char *dirpath,
		int (*fn)(const char *fpath, const struct stat *sb,
			  uint32_t level))
{
	TreeWalker walker;
	if (walker_init(&walker, dirpath) == -1) {
		return -1;
	}

	errno = 0;
	Node *item = NULL;
	while ((item = walker_next(&walker)) != NULL) {
		int32_t res = fn(item->path, &(item->stat_buf), item->level);
		if (res == -1) {
			free(item);
			return -1;
		}
		free(item);
	}

	if (errno != 0) {
		return -1;
	}

	return 0;
}
