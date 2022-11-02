#define _GNU_SOURCE

#include "utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>

int inotify;
Mappings mappings;
uint32_t mask = IN_CREATE | IN_DELETE_SELF | IN_DELETE | IN_MOVE |
		IN_MOVE_SELF | IN_EXCL_UNLINK;


int main(int ac, char *av[])
{
	if (ac != 2) {
		fprintf(stderr, "Usage: watch dir\n");
		exit(EXIT_FAILURE);
	}

	inotify = inotify_init();
	if (inotify == -1) {
		fprintf(stderr, "inotify_init(2)");
		exit(EXIT_FAILURE);
	}
	mappings_init(&mappings);
	char target_dir[PATH_MAX];
	if (NULL==realpath(av[1], target_dir)){
		fprintf(stderr, "realpath(3)\n");
		exit(EXIT_FAILURE);
	}

	int wd = inotify_add_watch(inotify, target_dir, mask);
	if (wd == -1) {
		fprintf(stderr, "inotify_add_watch(2)");
		exit(EXIT_FAILURE);
	}
	mappings_add(&mappings, entry_new(wd, target_dir, 1));

	nftw(target_dir, iterate_over_dir, 20, 0);

	char buf[1024];
	memset(buf, 0, 1024);
	ssize_t num_read = 0;
	for (;;) {
		num_read = read(inotify, buf, 1024);
		// we are using blocking I/O, `read(2)` should not return 0,
		if (num_read == 0) {
			fprintf(stderr, "read(2) from inotify fd return 0!");
			exit(EXIT_FAILURE);
		}
		if (num_read == -1) {
			fprintf(stderr, "read(2)");
			exit(EXIT_FAILURE);
		}

		// Process all of the events in buffer returned by read(2)
		char *p = buf;
		struct inotify_event *event = NULL;
		for (; p < (buf + num_read);) {
			event = (struct inotify_event *)p;
			handle_event(event, inotify, &mappings);
			p += sizeof(struct inotify_event) + event->len;
		}
	}
}
