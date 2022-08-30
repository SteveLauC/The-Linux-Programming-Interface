#### Question:
modify the program in Listing 5-3 to use standard file I/O system calls
(open() and lseek()) and `off_t` data type. Compile the program with the 
`_FILE_OFFSET_BITS` macro set to 64, and test it to show that a large file can
be successfully created.


#### Build:
```shell
$ gcc 5-1.c ../../../lib/error_functions.c -o 5-1
```

#### Test

```shell
$ ./5-1 test 3221225472
$ ls -l test
-rw------- 1 steve steve 3221225476 Jul 17 10:03 test
```
