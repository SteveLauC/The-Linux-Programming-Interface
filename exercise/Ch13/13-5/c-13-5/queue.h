#ifndef C_13_5_LIST_H
#define C_13_5_LIST_H

#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
	char *data;
	struct node *next;
} node;

typedef struct {
	node *head;
	int count;
} queue;

queue new_queue(void);

void push(queue *, char *, long);

void pop(queue *);

void clear(queue *);

#endif
