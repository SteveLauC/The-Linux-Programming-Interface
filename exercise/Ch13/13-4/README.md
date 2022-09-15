#### Question

Explain why the output of the following code differs depending on whether
standard output is redirected to a terminal or a disk file.

```c
printf("If I had more time, \n");
write(STDOUT_FILENO, "I would have written you a shorter letter.\n", 43);
```

#### Answer

When `stdout` refers to a terminal, it is `line-buffered`, which means the tailing
`\n` in `If I had more time, \n` will flush the library buffer, making this sentence
to be printed first.

Regular disk file is `block-buffered`, which means `If I had more time, \n` will only
be flushed when `stdout` is closed (i.e., the termination of this process).

```shell
$ gcc main.c

$ ./a.out # line-buffered
If I had more time,
I would have written you a shorter letter.

$ ./a.out > res && cat res # block-buffered
I would have written you a shorter letter.
If I had more time,
```
