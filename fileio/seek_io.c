/*
 * seek_io.c: a program to demostrate the usage of `lseek(2)`
 *
 * usage: seek_io file {options} 
 * 	
 * options: 
 * 	soffset: seek to byte offset from the start of the file
 * 	rlength: read length bytes from the file, starting at the current
 * 		file offset, and display them in text form
 * 	Rlength: read length bytes from the file, starting at the current
 * 		file offset, and display them in hexadecimal form
 * 	wstr: write the string of characters specified in str at the current
 * 		file offset
 *
 * compile: gcc seek_io.c ../lib/get_num.c ../lib/error_functions.c -o seek_io
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "../lib/tlpi_hdr.h"

int main(int ac, char *av[])
{
	size_t len;
	off_t offset;
	int fd, ap, j;
	char *buf = NULL;
	ssize_t numRead, numWritten;

	if (ac < 3 || strcmp(av[1], "--help") == 0) {
		usageErr(
			"%s file {r<length>|R<length>|w<string>|s<offset>}...\n",
			av[0]);
	}

	fd = open(av[1], O_RDWR | O_CREAT, 0666);
	if (fd == -1) {
		errExit("open");
	}

	// iterate over all the operation options
	for (ap = 2; ap < ac; ap++) {
		switch (av[ap][0]) {
		case 'r': // display bytes at current offset, as text
		case 'R': // display bytes at current offset, in hex
			len = getLong(&av[ap][1], GN_ANY_BASE, av[ap]);
			buf = malloc(len);
			if (buf == NULL) {
				errExit("malloc");
			}
			numRead = read(fd, buf, len);
			if (numRead == -1) {
				errExit("read");
			}
			if (numRead == 0) {
				printf("%s: end-of-file\n", av[ap]);
			} else {
				printf("%s: ", av[ap]);
				for (j = 0; j < numRead; j++) {
					if (av[ap][0] == 'r') {
						printf("%c ",
						       isprint((unsigned char)
								       buf[j]) ?
								     buf[j] :
								     '?');
					} else {
						printf("%02x ",
						       (unsigned int)buf[j]);
					}
				}
				printf("\n");
			}
			free(buf);
			buf = NULL;
			break;
		case 'w': // write string at current offset
			numWritten = write(fd, &av[ap][1], strlen(&av[ap][1]));
			if (numWritten == -1) {
				errExit("write");
			}
			printf("%s: wrote %ld bytes\n", av[ap],
			       (long)numWritten);
			break;
		case 's':
			offset = getLong(&av[ap][1], GN_ANY_BASE, av[ap]);
			if (lseek(fd, offset, SEEK_CUR) == -1) {
				errExit("lseek");
			}
			printf("%s: seek succeeded\n", av[ap]);
			break;
		default:
			cmdLineErr("Argument must start with [rRws]: %s\n",
				   av[ap]);
		}
	}

	exit(EXIT_SUCCESS);
}
