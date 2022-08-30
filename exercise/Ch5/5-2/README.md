#### Question

write a program that opens an existing file for writing with the 
`O_APPEND` flag, and then seeks to the beginning of the file before writing
any data. Where does the data appear in the file? Why?

#### Answer

The data will be written to the end of the file for the reason for
if a file is opened with `O_APPEND` flag, the offset will always be repositioned
to the end before any `write(2)` call.

#### Build

```shell
$ gcc 5-2.c ../../../lib/error_functions.c -o 5-2
```
#### Test:

```shell
$ echo "hello" > test
$ cat test
hello

$ gcc 5-2.c ../../../lib/error_functions.c -o 5-2
$ ./5-2 test
$ cat test
hello
test
```
