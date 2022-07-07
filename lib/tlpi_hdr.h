#ifndef TLPI_HDR_H
#define TLPI_HDR_H

#include <sys/types.h> /* Prevent accidental double inclusion */
#include <stdio.h> /* Type definitions used by many arguments */
#include <stdlib.h>
#include <unistd.h> /* Prototypes of commonly used library functions, plus EXIT_SUCCESS and EXIT_FAILURE */
#include <errno.h> /* Prototypes for many syscalls */
#include <string.h> /* Commonly used string-handling functions */
#include "get_num.h" /* Declares our functions for handling numeric arguments ( getInt(), getLong() ) */
#include "error_functions.h" /* Declares our error-handling functions */

typedef enum {
	FALSE,
	TRUE,
} Boolean;

#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#endif
