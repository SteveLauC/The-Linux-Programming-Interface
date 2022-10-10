#### Question

The chmod a+rX file command enables `read` permission for all categories of user,
and likewise enables execute permission for all categories of user if the file 
is a directory or execute permission is enabled for any of the user categories
for file, as demonstrated in the following example:

```shell
$ ls -ld dir fle prog
dr-------- 2 mtk users    48 May 4 12:28 dir
-r-------- 1 mtk users 19794 May 4 12:22 file
-r-x------ 1 mtk users 19336 May 4 12:21 prog

$ chmod a+rX dir file prog

$ ls -ld dir fle prog
dr-xr-xr-x 2 mtk users    48 May 4 12:28 dir
-r--r--r-- 1 mtk users 19794 May 4 12:22 file
-r-xr-xr-x 1 mtk users 19336 May 4 12:21 prog
```

Write a program that uses `stat(2)` and `chmod(2)` to perform the equivalent of 
`chmod a+rX`.


#### Verificatoin

```shell
$ cd c-impl
$ gcc main.c

$ mkdir dir
$ touch file prog
$ chmod 400 dir
$ chmod 400 file
$ chmod 500 prog

$ l -d dir file prog
Permissions Links Size User  Group Date Modified Name
dr--------@     1    - steve steve 10 Oct 12:34  dir
.r--------@     1    0 steve steve 10 Oct 12:34  file
.r-x------@     1    0 steve steve 10 Oct 12:34  prog

$ ./a.out dir file prog

$ l -d dir file prog
Permissions Links Size User  Group Date Modified Name
dr-xr-xr-x@     1    - steve steve 10 Oct 12:34  dir
.r--r--r--@     1    0 steve steve 10 Oct 12:34  file
.r-xr-xr-x@     1    0 steve steve 10 Oct 12:34  prog
```
