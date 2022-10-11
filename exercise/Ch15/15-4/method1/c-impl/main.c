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
	gid_t old_r_gid = getgid();
	uid_t e_uid = geteuid();
	gid_t e_gid = getegid();
	assert(0 == setresuid(e_uid, -1, -1));
	assert(0 == setresgid(e_gid, -1, -1));
	int ret = access(path, mode);
	assert(0 == setresuid(old_r_uid, -1, -1));
	assert(0 == setresgid(old_r_gid, -1, -1));

	return ret;
}
