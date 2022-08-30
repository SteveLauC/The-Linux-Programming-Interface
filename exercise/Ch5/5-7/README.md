#### QUESTION

Implement readv() and writev() using read() and write(), and suitable 
functions from the malloc package

#### TEST:

```shell
$ gcc 5-7.c -o 5.7
$ echo "hello world" > source
$ ls -l source
-rw-rw-r-- 1 steve steve 12 Jul 19 15:55 source
$ ./5-7
request 20 bytes, read 12 bytes
hello world
write 20 bytes to stdout
```
