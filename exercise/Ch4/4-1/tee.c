/*
 * tee.c: a tee implementation using universal I/O
 *
 * functionality: read from stdin, and write all the bytes read to stdout and 
 *  	          file options
 *
 * usage: tee [-a] file
 *
 * exit: use EOF(Ctrl+D) to exit the program
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

int main(int ac, char *av[])
{
	if (ac < 2 || ac > 3) {
		fprintf(stderr, "usage: tee [-a] file");
		exit(EXIT_FAILURE);
	}

	int append = 0;
	const char *file = NULL;
	for (int i = 1; i < ac; i++) {
		if (strcmp(av[i], "-a") == 0) {
			append = 1;
		} else {
			file = av[i];
		}
	}
	assert(file != NULL);
	printf("DEBUG: file %s\n", file);
	int flag = O_CREAT | O_WRONLY | (append ? O_APPEND : O_TRUNC);
	int fd = open(file, flag, 0666);
	if (fd == -1) {
		fprintf(stderr, "can not open or create file\n");
		perror("open");
		exit(EXIT_FAILURE);
	}

	int num_read = 0;
	char buf[BUFSIZ];
	while ((num_read = read(0, buf, BUFSIZ)) > 0) {
		if (num_read != write(STDOUT_FILENO, buf, num_read)) {
			fprintf(stderr, "can not write the whole buffer");
			exit(EXIT_FAILURE);
		}
		if (num_read != write(fd, buf, num_read)) {
			fprintf(stderr, "can not write the whole buffer");
			exit(EXIT_FAILURE);
		}
	}

	if (num_read == -1) {
		fprintf(stderr, "can not read all the contents");
		perror("read");
		exit(EXIT_FAILURE);
	}

	return 0;
}
