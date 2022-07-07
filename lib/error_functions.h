#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

// print a message on standard error, a newline character will be appended to the
// output string automatically
void errMsg(const char *format, ...);

#ifdef __GNUC__

/* This macro stops `gcc -Wall` complaining that "control reaches end of non-void functio"
*    if we use the following functions to terminate main() or some other non-void functions*/
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif

// like `errMsg()` but also terminates the program
void errExit(const char *format, ...) NORETURN;
void err_exit(const char *format, ...) NORETURN;
void errExitEN(int errnum, const char *format, ...) NORETURN;
void fatal(const char *format, ...) NORETURN;
void usageErr(const char *format, ...) NORETURN;
void cmdLineErr(const char *format, ...) NORETURN;

#endif
