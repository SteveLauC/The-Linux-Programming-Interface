#include <limits.h>
#include <sys/stat.h>
#include <stdint.h>

#define STACK_MAX_LEN 100

typedef struct {
	char path[PATH_MAX];
	struct stat stat_buf;
	uint64_t level;
}Node;

uint32_t node_is_dir(Node const *);

typedef struct {
	Node *data[STACK_MAX_LEN];
	uint32_t len;
}Stack;

void stack_init(Stack *);
Node * stack_pop(Stack *);
int32_t stack_push(Stack *, Node *);

typedef struct {
	Stack stack;
}TreeWalker;

int32_t walker_init(TreeWalker *, const char *);
Node * walker_next(TreeWalker *);