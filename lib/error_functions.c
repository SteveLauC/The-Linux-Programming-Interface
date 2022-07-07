#include <stdarg.h>
#include "error_functions.h"
#include "tlpi_hdr.h"
#include "ename.c.inc"

/*
 * purpose: terminate the program
 *
 * action: if `EF_DUMPCORE` is validly set, call `abort()`. Otherwise, call `exit`
 *         or `_exit()` depending on `useExit3`
 *
 * arguments:
 *  * `useExit3`: boolean to represent if we use `exit` over `_exit`
 *
 * note: static function
*/
#ifdef __GNUC__
__attribute__((noreturn))
#endif
static void
terminate(Boolean useExit3)
{
	char *s;
	/* Dump core if `EF_DUMPCORE` environment variable is defined and is a nonempty
     * string; Otherwise, call exit(3) or _exit(2), depanding on the value of 
     * `useExit3` */
	s = getenv("EF_DUMPCORE");
	if (s != NULL && *s != '\0') {
		abort();
	} else if (useExit3) {
		exit(EXIT_FAILURE);
	} else {
		_exit(EXIT_FAILURE);
	}
}

/* purpose: the underlying error print function
 *
 * action: print user message specified by `format` and `ap`, then print error 
 *         message.
 *
 * arguments:
 *  * `useErr`: determine if we should output the error message
 *  * `err`: errno number
 *  * `flushStdout`: whether flush stdout before print
 *  * `format`: format string of user message
 *  * `ap`: variadic arguments list
 *
 * note: 
 *  * for the variadic arguments passed by `ap`, we use `vsnprintf` to deal
 *    with it instead of using `va_arg` manually
 *  * static function's scope is limited to its compilation unit, which means
 *    you can not call this func from external files
*/
static void outputError(Boolean useErr, int err, Boolean flushStdout,
			const char *format, va_list ap)
{
#define BUF_SIZE 500
	char buf[BUF_SIZE];
	char userMsg[BUF_SIZE];
	char errText[BUF_SIZE];

	// construct `userMsg` string, implicitly call `va_arg`
	vsnprintf(userMsg, BUF_SIZE, format, ap);

	if (useErr) {
		snprintf(errText, BUF_SIZE, " [%s %s]",
			 (err > 0 && err <= MAX_ENAME) ? ename[err] :
							       "?UNKNOWN",
			 strerror(err));
	} else {
		snprintf(errText, BUF_SIZE, ":");
	}

	// final output string, which consists of `errText` and `userMsg`
	snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

	if (flushStdout) {
		fflush(stdout);
	}
	fputs(buf, stderr);
	fflush(stderr);
}

/*
 * purpose: print error message to stdout
 *
 * action: call `outputError`
 *
 * arguments:
 *  * `format`: user message
 *
 *  note: this func is a variadic function
 *
*/
void errMsg(const char *format, ...)
{
	va_list argList;
	int savedErrno;

	savedErrno = errno;

	va_start(argList, format);
	outputError(TRUE, errno, TRUE, format, argList);
	va_end(argList);

	errno = savedErrno;
}

/*
 * purpose: print error message and exit
 *
 * action: call `outputError` and `terminate`
 *
 * arguments:
 * * `format`: user message
 *
 * note: variadic function
*/
void errExit(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	outputError(TRUE, errno, TRUE, format, argList);
	va_end(argList);

	terminate(TRUE);
}

/*
 * purpose: print error message and exit
 *
 * action: call `outputError` and `terminate`
 *
 * arguments:
 * * `format`: user message
 *
 * note: 
 * * the differences between `errExit` and `err_exit` are that
 *  1. `errExit` flushes stdout while `err_exit` does not do this
 *  2. `errExit` uses `exit` and `err_exit` uses `_exit` in the underlying `terminate`
 *      call.
 * * variadic function
*/
void err_exit(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	outputError(TRUE, errno, FALSE, format, argList); // diff1
	va_end(argList);

	terminate(FALSE); // diff2
}

/*
 * purpose: print error message and exit
 *
 * action: call `outputError` and `terminate`
 *
 * arguments:
 * * `format`: user message
 *
 * note:
 *  * `errExitEN` is like `errExit` but does not invoke `errno` directly
 *      so that we can reduce the number of syscalls
 *  * variadic function
*/
void errExitEN(int errnum, const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	outputError(TRUE, errnum, TRUE, format, argList);
	va_end(argList);

	terminate(TRUE);
}

/*
 * purpose: error message print function but for functions that won't invoke 
 *          `errno` when encounters error
 *
 * action: call `outputError` and `terminate`
 *
 * arguments:
 *  * `format`: user message
 *
 * note: variadic function
*/
void fatal(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	outputError(FALSE, 0, TRUE, format, argList);
	va_end(argList);

	terminate(TRUE);
}

/*
 * purpose: diagnose errors in command-line argument usage
 *
 * action: print `Usgae: ` + function arguments to stderr
 *
 * arguments:
 *  * `format`: error message
 *
 * note: variadic function
*/
void usageErr(const char *format, ...)
{
	va_list argList;

	fflush(stdout); /* flush any pending stdout */

	fprintf(stderr, "Usage: ");
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);

	fflush(stderr); /* In case stderr is not line-buffered */
	exit(EXIT_FAILURE);
}

/*
 * purpose: diagnose errors in the command-line arguments specified to a program
 *
 * action: print "Command-line usage error: " and error info to stderr
 *
 * arguments:
 *  * `format`: error message
 *
 * note: variadic function
*/
void cmdLineErr(const char *format, ...)
{
	va_list argList;

	fflush(stdout); /* flush any pending stdout */

	fprintf(stderr, "Command-line usage error: ");
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);

	fflush(stderr); /* In case stderr is not line-buffered */

	exit(EXIT_FAILURE);
}
