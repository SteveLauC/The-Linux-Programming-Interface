/*
 * QUESTION: 
 * This exercise is designed to demonstrate why the atomicity guaranteed by 
 * opening a file with O_APPEND flag is necessary. Write a program that takes
 * up 3 command-line arguments:
 *
 * 	$ atomic_append filename num-bytes [x]
 *
 * The file should open the specified filename(creating it if necessary) and
 * `append` num-bytes to the file with O_APPEND flag, but if a third argument
 * is supplied, then the O_APPEND flag should be omitted, and instead the program
 * should perform lseek(fd, 0, SEEK_END) call before write. Run two instances
 * of this program at the same time without x argument to write 1 million bytes
 * to the same file:
 *
 * 	$ atomic_append f1 1000000 & atomic_append f1 1000000
 * 
 * Repeat the same steps, writing to a different file, but this time specifying
 * the x argument:
 *
 * 	$ atomic_append f1 1000000 x & atomic_append f1 1000000 x
 *
 * ANSWER:
 * If the third argument is given, the atomicity of appending data is disabled.
 * And 2 processes are writing to the same file at the same time, one process may 
 * overwrite the data written by another one. So you may find the total size
 * of this file is smaller than 1000000 * 2.
 *
 * TEST:
 * 	$ gccs ../../../lib/error_functions.c ../../../get_num.c -o atomic_append
 * 	$ atomic_append f1 1000000 & atomic_append f1 1000000
 * 	$ ls -l f1
 *	-rw-rw-r-- 1 steve steve 2000000 Jul 18 07:18 f1
 * 	$ atomic_append f1 1000000 x & atomic_append f1 1000000 x
 * 	$ ls -l f2
 *	-rw-rw-r-- 1 steve steve 1157196 Jul 18 07:18 f2(file size is SMALLER than 2000000)
*/

#include "../../../lib/tlpi_hdr.h"
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

int main(int ac, char *av[])
{
	if (ac < 3 || ac > 4) {
		usageErr("usage: %s filename num-bytes [x]", av[0]);
	}

	int flag = 0;
	int atomic = 0;
	if (ac == 3) {
		flag = O_CREAT | O_WRONLY | O_APPEND;
		atomic = 1;
	} else if (ac == 4) {
		flag = O_CREAT | O_WRONLY;
	}

	int fd = open(av[1], flag, 0666);
	if (fd == -1) {
		errExit("open");
	}

	int num_bytes = getInt(av[2], GN_ANY_BASE, NULL);

	for (int i = 0; i < num_bytes; i += 1) {
		if (!atomic) {
			lseek(fd, 0, SEEK_END);
		}

		assert(1 == write(fd, "h", 1));
	}

	close(fd);
}
