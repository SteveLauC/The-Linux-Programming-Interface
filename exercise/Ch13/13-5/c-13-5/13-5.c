/*
 * usage: 13-5 [-n] FILE
 *
 * implementation: Always keep the latest `n` lines while reading the file, print
 * them out and exit when encounters EOF.
 */

#include "queue.h"
#include "utility.h"
#include <fcntl.h>

// need a big buffer to decrease the number of `read(2)`
#define BUF_SIZ (BUFSIZ * 5)

int main(int ac, char *av[])
{
	char *filename = NULL;
	int n = 10;
	arg_init(&filename, &n, ac, av);
	queue q = new_queue();
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("open(2)");
		exit(EXIT_FAILURE);
	}

	char buf[BUF_SIZ] = { 0 };
	ssize_t num_read = 0;

	while ((num_read = read(fd, buf, BUF_SIZ)) != -1) {
		// EOF
		if (num_read == 0) {
			break;
		}
		buf[num_read] = '\0'; // add the missing NUL

		int line_count = num_occurrences(buf, '\n');
		if (line_count > 1) {
			trim_buf(buf, fd);
		}

		char *start = NULL;
		long len = 0;
		for (int i = (line_count > n) ? (line_count - n + 1) : 1;
		     i <= line_count; i += 1) {
			assert(nth_line(buf, i, &start, &len) == 0);
			push(&q, start, len);
		}

		while (q.count > n) {
			pop(&q);
		}
	}

	assert(q.count <= n);
	node *p = q.head;
	for (int i = 0; i < q.count; i += 1) {
		printf("%s\n", p->data);
		p = p->next;
	}

	clear(&q);
}
