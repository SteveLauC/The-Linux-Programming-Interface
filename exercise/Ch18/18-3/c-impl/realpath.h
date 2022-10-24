#ifndef REALPATH_H
#define REALPATH_H


#include <limits.h>
#include <errno.h>
#include "utility.h"
#include <sys/stat.h>
#include <unistd.h>


char * my_realpath(const char* , char *);


#endif