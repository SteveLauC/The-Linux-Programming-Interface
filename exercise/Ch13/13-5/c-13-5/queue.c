#include "queue.h"

queue new_queue(void)
{
	queue q;
	q.head = NULL;
	q.count = 0;
	return q;
}

void push(queue *q, char *src, long len)
{
	node *n = (node *)malloc(sizeof(node));
	assert(n != NULL);
	memset(n, 0, sizeof(node));

	n->data = (char *)malloc(len + 1);
	assert(n->data != NULL);
	memset(n->data, 0, len + 1);

	memcpy(n->data, src, len);
	*((n->data) + len) = '\0';
	n->next = NULL;

	if (q->head == NULL) {
		q->head = n;
	} else {
		node *cursor = q->head;
		// move the cursor to the last node
		for (int i = 1; i < q->count; i += 1) {
			cursor = cursor->next;
		}
		cursor->next = n;
	}

	q->count += 1;
}

void pop(queue *q)
{
	node *n = q->head;
	// 1. free the string
	free(n->data);
	n->data = NULL;

	q->head = n->next;

	// 2. free the node
	free(n);

	q->count -= 1;
}

void clear(queue *q)
{
	node *p = q->head;
	for (int i = 0; i < q->count; i += 1) {
		node *tmp = p;
		p = p->next;
		free(tmp->data);
		free(tmp);
	}
}
