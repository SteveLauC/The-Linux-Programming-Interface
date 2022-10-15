#### Question

Write a program that can be used to create or modify a user EA for a file (i.e., 
a simple version of setfattr(1)). The filename and the EA name and value should 
be supplied as command-line arguments to the program.

#### Answer

```shell
$ cd c-impl
$ gccs main.c

$ ./a.out -n "user.first_ea" -v "first_ea" /home/steve/Desktop/file
$ getfattr -d /home/steve/Desktop/file
getfattr: Removing leading '/' from absolute path names
# file: home/steve/Desktop/file
user.first_ea="fist_ea"

$ ./a.out -n "user.first_ea" -v "ooops" /home/steve/Desktop/file 
$ getfattr -d /home/steve/Desktop/file
getfattr: Removing leading '/' from absolute path names
# file: home/steve/Desktop/file
user.first_ea="ooops"
```
