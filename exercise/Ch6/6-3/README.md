#### QUESTION

Implement `setenv()` and `unsetenv()` using `getenv()`, `putenv()`,
and, where necessary, code that directly modifies `environ`. Your version of
`unsetenv()` should check to see whether there are multiple definitons of an
environment variable, and remove them all (which is what glibc version of 
`unsetenv()` does).
#### TEST

```shell
$ gcc 6-3.c ../../../lib/error_functions.c -0 6-3
$ ./6-3
$ echo $?
0   # all tests passed
```
#### NOTE
If the argumnet of `putenv()` does not have the format of "name=value",
it will simply return 0 :(

And the memory allocated in `allocation()` is not freed. This book says the
official `setenv()` also have memory leak issue, I tested it and did not any
memory leak.

You can use `valgrind --leak-check=yes ./6-3` to check memory leak
