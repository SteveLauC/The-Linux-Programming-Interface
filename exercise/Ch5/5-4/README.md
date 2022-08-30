#### QUESTION

Implement `dup()` and `dup2()` using `fcntl()` and, where necessary,
`close()`. (You may ignore the fact that dup2() and fcntl() return different
errno values for some error cases.) For `dup2()`, remeber to handle the spcial
case where oldfd equals newfd. In this case, you should check whether oldfd is 
valid, which can be done by, for example, checking if `fcntl(fd, F_GETFL)` 
succeeds. If oldfd is not valid, then the function should return -1 with errno
set to EBADF

#### TEST

In function main, I redirect stdout to the file specified by name av[1]
and stderr to file specified by av[2], then flush some bytes to stdout and stderr

```shell
$ gcc 5-4.c ../../../lib/error_functions.c -o 5-4
$ ./5-4 stdout stderr
$ cat stdout
hello stdout

$ cat stderr
hello stderr
```

#### ISSUE

In `my_dup2`, if `new_open_fd` does not equals to `newfd`, the function
will return `-1`, but errno will not be modified.
