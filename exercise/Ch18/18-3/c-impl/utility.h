#ifndef UTILITY_H
#define UTILITY_H

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

struct Paths {
	char parsed[PATH_MAX];
	char remained[PATH_MAX];
	// idx of last char + 1
	int parsed_len;
	// The first char of `remained`
	int remained_start;
	// idx of last char + 1
	int remained_after_end;
};

void paths_init(struct Paths *, const char *, const char *);
int is_absolute(const char *);
int is_slash(const char *);
char *remove_extra_slash(const char *);
int is_a_pair_of_dot(const char *);
int is_dot(const char *);
void remained_push_front(struct Paths *, const char *);
char *remained_next_entry(struct Paths *);
void replace_parsed(struct Paths *, const char *);
void parsed_push_back(struct Paths *, const char *);
void parsed_cd_to_parent(struct Paths *);

#endif
