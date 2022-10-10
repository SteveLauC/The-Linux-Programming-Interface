#include <stdio.h>
#include <sys/stat.h>

static int chmod_a_r_x(const char *);
inline static int executable(mode_t);

int main(int ac, char *av[])
{
	for (int i = 1; i < ac; i += 1) {
		if (chmod_a_r_x(av[i]) == -1) {
			fprintf(stderr, "failed on %s\n", av[i]);
			continue;
		}
	}
}

// return 1 if `x` is set for any categories of users. Otherwise, return 0.
inline static int executable(mode_t mode)
{
	return (mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH);
}

// 1. retrieve previous st_mode
// 2. grant `r` to all categories of users
// 3. if `file` is a dir  grant `x` to all categories of users
// 4. else if executable: grant `x` to all categories of users
static int chmod_a_r_x(const char *file)
{
	struct stat buf;
	if (-1 == stat(file, &buf)) {
		return -1;
	}

	mode_t mode = buf.st_mode;
	// grant `r` permission
	mode |= S_IRUSR;
	mode |= S_IRGRP;
	mode |= S_IROTH;

	if (S_ISDIR(mode)) {
		mode |= S_IXUSR;
		mode |= S_IXGRP;
		mode |= S_IXOTH;
	} else if (executable(mode)) {
		mode |= S_IXUSR;
		mode |= S_IXGRP;
		mode |= S_IXOTH;
	}

	return chmod(file, mode);
}