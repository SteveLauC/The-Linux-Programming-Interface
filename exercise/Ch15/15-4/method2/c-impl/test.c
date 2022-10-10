#include "test.h"

void file_that_does_not_exist(void)
{
	assert(my_eaccess("file_that_does_not_exist", F_OK) == -1);
	assert(my_eaccess("file_that_does_not_exist", F_OK | R_OK) == -1);
	assert(my_eaccess("file_that_does_not_exist", F_OK | R_OK | W_OK) ==
	       -1);
	assert(my_eaccess("file_that_does_not_exist",
			  F_OK | R_OK | W_OK | X_OK) == -1);
}

void with_no_permission(void)
{
	unlink("file");
	assert(-1 != open("file", O_CREAT | O_EXCL | O_RDWR, 00));
	assert(0 == my_eaccess("file", F_OK));
	// if privileged, `rw` should be granted
	if (geteuid() == 0) {
		assert(0 == my_eaccess("file", F_OK | R_OK));
		assert(0 == my_eaccess("file", F_OK | R_OK | W_OK));
	} else {
		assert(-1 == my_eaccess("file", F_OK | R_OK));
		assert(-1 == my_eaccess("file", F_OK | R_OK | W_OK));
	}
	// `x` is not granted no matter whether this process is privileged
	assert(-1 == my_eaccess("file", F_OK | R_OK | W_OK | X_OK));
	assert(0 == unlink("file"));
}

void with_read_permission(void)
{
	unlink("file");
	assert(-1 != open("file", O_CREAT | O_EXCL | O_RDWR, 0400));
	assert(0 == my_eaccess("file", F_OK));
	assert(0 == my_eaccess("file", R_OK));
	assert(-1 == my_eaccess("file", X_OK));

	if (geteuid() == 0) {
		assert(0 == my_eaccess("file", W_OK));
	} else {
		assert(-1 == my_eaccess("file", W_OK));
	}

	assert(-1 == my_eaccess("file", F_OK | R_OK | W_OK | X_OK));
	assert(0 == unlink("file"));
}

void with_read_write_permission(void)
{
	unlink("file");
	assert(-1 != open("file", O_CREAT | O_EXCL | O_RDWR, 0600));
	assert(0 == my_eaccess("file", F_OK));
	assert(0 == my_eaccess("file", R_OK));
	assert(0 == my_eaccess("file", W_OK));
	assert(0 == my_eaccess("file", R_OK | W_OK));
	assert(-1 == my_eaccess("file", R_OK | X_OK));
	assert(-1 == my_eaccess("file", F_OK | R_OK | W_OK | X_OK));
	assert(0 == my_eaccess("file", F_OK | R_OK | W_OK));
	assert(0 == unlink("file"));
}

void with_read_write_execute_permission(void)
{
	unlink("file");
	// with default umask `0222`, this new file should have permission `0755`
	assert(-1 != open("file", O_CREAT | O_EXCL | O_RDWR, 0777));
	assert(0 == my_eaccess("file", F_OK));

	assert(0 == my_eaccess("file", R_OK));
	assert(0 == my_eaccess("file", R_OK | W_OK));
	assert(0 == my_eaccess("file", R_OK | W_OK | X_OK));

	assert(0 == unlink("file"));
}