#### QUESTION

modify the program in listing 7-1(free_and_sbrk.c) to print out the
current value of the program break after each execution of `malloc()`. Run the
program specifying a small allocation block size. This will demonstrate that
`malloc()` doesn't employ `sbrk()` to adjust the program break on each call, 
but instead periodically allocates larger chunks of memory from which it passes
back small pieces to the caller.

#### TEST

```shell
$ gcc 7-1.c ../../../lib/error_functions.c ../../../lib/get_num.c -o 7-1
$ ./7-1 20 10240 > result
$ cat result
Initial program break:       0x55cc3977f000
Allocating 20*10240 bytes
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc3977f000
(During allocation)Program break is:       0x55cc397a0000
(During allocation)Program break is:       0x55cc397a0000
(During allocation)Program break is:       0x55cc397a0000
(During allocation)Program break is:       0x55cc397a0000
(During allocation)Program break is:       0x55cc397a0000
(During allocation)Program break is:       0x55cc397a0000
(During allocation)Program break is:       0x55cc397a0000
(During allocation)Program break is:       0x55cc397a0000
Program break is now:       0x55cc397a0000
Freeing blocks from 1 to 20 in steps of 1
After free(), program break is       0x55cc39780000
```
