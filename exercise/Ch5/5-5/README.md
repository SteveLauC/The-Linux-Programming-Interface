#### QUESTION

write a program to verify that duplicated file descriptors share a 
file offset value and open file status flags

#### ANSWER 

We will open a file, and duplicate the file descriptor, then print the offsets
of these file descriptors. And write some bytes to one of the file descriptors
then print the offsets again.

For the status flag, we will do pretty much the same thing. Fetch status 
flags of these two file descriptors and modify one of them, then print them 
again.

#### TEST:

```shell
$ gccs 5-5.c ../../../lib/error_functions.c -o 5-5
$ ./5-5
before modification: offset1: 0 | offset2: 0
writing 5 bytes to fd1...
after modification: offset1: 5 | offset2: 5

before modification: flag1 has O_NONBLOCK 0| flag2 has O_NONBLOCK 0
setting O_NONBLOCK flag to fd1...
after modification: flag1 has O_NONBLOCK 1| flag2 has O_NONBLOCK 1
```
