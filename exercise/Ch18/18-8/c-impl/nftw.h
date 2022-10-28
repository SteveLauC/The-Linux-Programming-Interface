#include <stdint.h>
#include <sys/stat.h>

int32_t my_nftw(const char *,
		int (*)(const char *fpath, const struct stat *sb, uint32_t));