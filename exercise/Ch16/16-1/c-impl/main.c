#include <unistd.h>
#include <string.h>
#include <sys/xattr.h>
#include <stdlib.h>
#include <stdio.h>

#define XATTR_SIZE 10000

int main(int ac, char *av[])
{
	char name[XATTR_SIZE] = { 0 };
	char value[XATTR_SIZE] = { 0 };

	int opt;
	while ((opt = getopt(ac, av, "n:v:")) != -1) {
		switch (opt) {
		case 'n':
			strncpy(name, optarg, strlen(optarg));
			break;
		case 'v':
			strncpy(value, optarg, strlen(optarg));
			break;
		default:
			fprintf(stderr, "Unsupported Option\n");
			exit(EXIT_FAILURE);
		}
	}

	if (optind > ac) {
		fprintf(stderr, "Usage: %s -n <name> -v <value> file", av[0]);
		exit(EXIT_FAILURE);
	}

	int res = setxattr(av[optind], name, value, strlen(value), 0);
	if (res == -1) {
		perror("setxattr: ");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}