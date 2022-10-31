#include <stdio.h>
#include <sys/inotify.h>
#include <limits.h>
#include "../../../lib/tlpi_hdr.h"

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

static void display_inotify_event(struct inotify_event *);

int main(int ac, char *av[])
{
	if (ac < 2 || strcmp(av[1], "--help") == 0) {
		usageErr("%s pathname...\n", av[0]);
	}
	int inotify_fd = inotify_init();
	if (inotify_fd == -1) {
		errExit("inotify_init");
	}

	for (int j = 0; j < ac; j += 1) {
		int wd = inotify_add_watch(inotify_fd, av[j], IN_ALL_EVENTS);
		if (wd == -1) {
			errExit("inotify_add_watch");
		}

		printf("Watching %s using wd %d\n", av[j], wd);
	}

	char buf[BUF_LEN] __attribute__((aligned(8)));
	memset(buf, 0, BUF_LEN);
	ssize_t num_read = 0;
	for (;;) {
		num_read = read(inotify_fd, buf, BUF_LEN);
		// we are using blocking I/O, `read(2)` should not return 0,
		if (num_read == 0) {
			fatal("read(2) from inotify fd return 0!");
		}
		if (num_read == -1) {
			errExit("read(2)");
		}

		printf("Read %ld bytes from inotify fd\n", (long)num_read);

		// Process all of the events in buffer returned by read(2)
		char *p = buf;
		struct inotify_event *event = NULL;
		for (; p < (buf + num_read);) {
			event = (struct inotify_event *)p;
			display_inotify_event(event);
			p += sizeof(struct inotify_event) + event->len;
		}
	}
}

// Display information from `struct inotify_event`
static void display_inotify_event(struct inotify_event *i)
{
	printf("    wd =%2d;", i->wd);
	if (i->cookie != 0) {
		printf("cookie = %4d;", i->cookie);
	}
	printf("mask = ");
	uint32_t mask = i->mask;
	if (mask & IN_ACCESS) {
		printf("IN_ACCESS ");
	}
	if (mask & IN_ATTRIB) {
		printf("IN_ATTRIB ");
	}
	if (mask & IN_CLOSE_NOWRITE) {
		printf("IN_CLOSE_NOWRITE ");
	}
	if (mask & IN_CLOSE_WRITE) {
		printf("IN_CLOSE_WRITE ");
	}
	if (mask & IN_CREATE) {
		printf("IN_CREATE ");
	}
	if (mask & IN_DELETE) {
		printf("IN_DELETE ");
	}
	if (mask & IN_DELETE_SELF) {
		printf("IN_DELETE_SELF ");
	}
	if (mask & IN_IGNORED) {
		printf("IN_IGNORED ");
	}
	if (mask & IN_ISDIR) {
		printf("IN_ISDIR ");
	}
	if (mask & IN_MODIFY) {
		printf("IN_MODIFY ");
	}
	if (mask & IN_MOVE_SELF) {
		printf("IN_MOVE_SELF ");
	}
	if (mask & IN_MOVED_FROM) {
		printf("IN_MOVED_FROM ");
	}
	if (mask & IN_MOVED_TO) {
		printf("IN_MOVED_TO ");
	}
	if (mask & IN_OPEN) {
		printf("IN_OPEN ");
	}
	if (mask & IN_Q_OVERFLOW) {
		printf("IN_Q_OVERFLOW ");
	}
	if (mask & IN_UNMOUNT) {
		printf("IN_UNMOUNT ");
	}
	printf("\n");

	if (i->len > 0) {
		printf("        name = %s\n", i->name);
	}
}
