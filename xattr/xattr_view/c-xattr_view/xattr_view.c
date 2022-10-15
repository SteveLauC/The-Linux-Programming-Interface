#include <sys/xattr.h>
#include "../../../lib/tlpi_hdr.h"

#define XATTR_SIZE 10000

static void usage_error(char *);

int main(int ac, char *av[])
{
	char list[XATTR_SIZE], value[XATTR_SIZE];
	ssize_t list_len, value_len;
	int ns, opt;
	Boolean hex_mode = FALSE;

	while ((opt = getopt(ac, av, "x")) != -1) {
		switch (opt) {
		case 'x':
			hex_mode = TRUE;
			break;
		default:
			usage_error(av[0]);
		}
	}

	if (optind > ac) {
		usage_error(av[0]);
	}

	list_len = listxattr(av[optind], list, XATTR_SIZE);
	if (list_len == -1) {
		errExit("lisxattr");
	}

	printf("%s:\n", av[optind]);

	for (ns = 0; ns < list_len; ns += strlen(&list[ns]) + 1) {
		printf("	name=%s;", &list[ns]);

		value_len = getxattr(av[optind], &list[ns], value, XATTR_SIZE);

		if (value_len == -1) {
			printf("couldn't get value");
		} else if (!hex_mode) {
			printf("value=%.*s\n", (int)value_len, value);
		} else {
			printf("value=");
			for (int i = 0; i < value_len; i += 1) {
				printf("%02x ", (unsigned char)value[i]);
			}
			printf("\n");
		}
	}

	exit(EXIT_SUCCESS);
}

static void usage_error(char *prog_name)
{
	fprintf(stderr, "Usage: %s [-x] file\n", prog_name);
	exit(EXIT_FAILURE);
}