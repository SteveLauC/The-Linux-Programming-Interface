#### Question

Write a simple version of the chattr(1) command, which modifies file i-node flags.
See the chattr(1) man page for details of the chattr command-line interface. (You 
don't need to implement the -R, -V, and -v options.)

#### Answer

```shell
$ gcc main.c

$ touch file
$ lsattr file
---------------------- file

$ sudo ./a.out +ai file
$ lsattr file
----ia---------------- file
```
