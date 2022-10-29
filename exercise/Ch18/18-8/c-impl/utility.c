#include "utility.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

// Does this `Node` represent a directory?
//
// Return 1 if it does, 0 if it does not.
inline uint32_t node_is_dir(Node const *self)
{
	assert(self != NULL);
	return S_ISDIR(self->stat_buf.st_mode);
}

// Pop an item from the stack
//
// User should de-allocate that `Node` using `free(3)`
Node *stack_pop(Stack *self)
{
	assert(self != NULL);

	if (self->len >= 1) {
		self->len -= 1;
		return self->data[self->len];
	}

	return NULL;
}

// Push `item` into the stack.
//
// Return -1 if the stack reaches is full (i.e., length reaches STACK_MAX_LEN)
int32_t stack_push(Stack *self, Node *item)
{
	assert(self != NULL && item != NULL);
	if (self->len == STACK_MAX_LEN) {
		return -1;
	}

	self->data[self->len++] = item;
	return 0;
}

void stack_init(Stack *self)
{
	memset(self->data, 0, (sizeof(Node *) * STACK_MAX_LEN));
	self->len = 0;
}

// Initialize a TreeWalker
//
// Return 0 on success, -1 on error, and errno is set to indicate the error.
int32_t walker_init(TreeWalker *self, const char *path)
{
	assert(self != NULL && path != NULL);
	stack_init(&(self->stack));

	Node *root = malloc(sizeof(Node));
	if (root == NULL) {
		return -1;
	}
	memset(root, 0, sizeof(Node));

	if (stat(path, &(root->stat_buf)) == -1) {
		return -1;
	}
	// use `realpath(3)` to get a clear, absolute path
	if (realpath(path, root->path) == NULL) {
		return -1;
	}
	root->level = 0;

	stack_push(&(self->stack), root);

	return 0;
}

// Return next entry
//
// Return NULL if the iteration is done, and errno is set to indicate the error.
//
// To distinguish the case where there are no more entries and an error case,
// clear errno before and check the value of errno if the return value is NULL.
Node *walker_next(TreeWalker *self)
{
	assert(self != NULL);

	Node *node = stack_pop(&self->stack);
	if (node == NULL) {
		return NULL;
	}

	if (node_is_dir(node)) {
		Stack temp_stack;
		stack_init(&temp_stack);

		DIR *dir_p = opendir(node->path);
		if (dir_p == NULL) {
			return NULL;
		}

		struct dirent *dirent_p = NULL;
		while ((dirent_p = readdir(dir_p)) != NULL) {
			// skip . and ..
			// We can guarantee `dirent_p->d_name` is NUL-terminated
			// use `strcmp` instead of `strncmp`
			if (strcmp(".", dirent_p->d_name) == 0 ||
			    strcmp("..", dirent_p->d_name) == 0) {
				continue;
			}

			Node *file = malloc(sizeof(Node));
			if (file == NULL) {
				closedir(dir_p);
				return NULL;
			}
			// Path concatenation
			int32_t dir_path_len = (int32_t)strlen(node->path);
			int32_t file_name_len =
				(int32_t)strlen(dirent_p->d_name);
			// 1. directory path
			strncpy(file->path, node->path, dir_path_len + 1);
			// 2. slash
			memcpy(file->path + dir_path_len, "/", 1);
			// 3. file name
			strncpy((file->path) + dir_path_len + 1,
				dirent_p->d_name, file_name_len + 1);
			// double check we have a tailing NUL
			file->path[dir_path_len + file_name_len + 1] = '\0';

			// file metadata
			if (stat(file->path, &file->stat_buf) == -1) {
				closedir(dir_p);
				free(file);
				return NULL;
			}
			// level
			file->level = (node->level + 1);

			if (stack_push(&temp_stack, file) == -1) {
				closedir(dir_p);
				free(file);
				return NULL;
			}
		}
		closedir(dir_p);

		// push files to the stack
		Node *item = NULL;
		while ((item = stack_pop(&temp_stack)) != NULL) {
			stack_push(&(self->stack), item);
		}
	}

	return node;
}
