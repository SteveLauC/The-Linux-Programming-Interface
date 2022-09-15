#ifndef C_13_5_UTILITY_H
#define C_13_5_UTILITY_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

int num_occurrences(const char *, char);

int nth_line(const char *, int, char **, long *);

void arg_init(char **, int *, int, char **);

void trim_buf(char *, int);

#endif
