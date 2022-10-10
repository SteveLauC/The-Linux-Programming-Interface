#define _GNU_SOURCE
#include <unistd.h>
#include <assert.h>

int my_eaccess(const char *, int);

int main(void)
{
	assert(0 == my_eaccess("file", R_OK));
}

int my_eaccess(const char *path, int mode)
{
	uid_t old_r_uid = getuid();
	uid_t e_uid = geteuid();
	assert(0 == setresuid(e_uid, -1, -1));
	int ret = access(path, mode);
	assert(0 == setresuid(old_r_uid, -1, -1));
	return ret;
}
