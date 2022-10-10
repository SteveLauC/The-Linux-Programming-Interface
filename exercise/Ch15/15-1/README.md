#### Question

Section 15.4 contained several statements about the permissions required for 
various file-system operations. Use shell commands or write programs to verify
or answer the following:

1. Removing all owner permissions from a file denies the file owner access, 
   even though group and other do have access. 

2. On a directory with read permission but not execute permission, the names
   of files in the directory can be listed, but the files themselves can't be
   accessed, regardless of the permissions on them. 

3. What permissions are required on the parent directory and the file itself 
   in order to create a new file, open a file for reading, open a file for 
   writing, and delete a file? 

   What permissions are required on the source and target directory to rename
   a file? If the target file of a rename operation already exists, what 
   permissions are required on that file? 

   How does setting the sticky permission bit (chmod +t) of a directory affect 
   renaming and deletion operations.

#### Answer

##### 1

When the EUID (fs UID, to be accurate) of the process matches the owner of a 
file, then the permissions for owner are granted.

```shell
$ echo "echo \"hello\"" >> test
$ l test
Permissions Links Size User  Group Date Modified Name
.rw-r--r--@     1   13 steve steve  9 Oct 11:21  test
$ chmod 077 test
$ l test
Permissions Links Size User  Group Date Modified Name
.---rwxrwx@     1   13 steve steve  9 Oct 11:21  test
$ id
uid=1000(steve) gid=1000(steve) groups=1000(steve),10(wheel),974(docker) context=unconfined_u:unconfined_r:unconfined_t:s0-s0:c0.c1023

$ cat test
cat: test: Permission denied

$ echo "append" >> test
warning: An error occurred while redirecting file 'test'
open: Permission denied

$ ./test
fish: Unknown command. '/home/steve/Desktop/test' exists but is not an executable file.
```

##### 2

```shell
$ mkdir test_dir
$ l -d test_dir/
Permissions Links Size User  Group Date Modified Name
drwxr-xr-x@     1    - steve steve  9 Oct 11:24  test_dir
$ echo "contents" > test_dir/test
$ l test_dir/test
Permissions Links Size User  Group Date Modified Name
.rw-r--r--@     1    9 steve steve  9 Oct 11:25  test_dir/test
$ chmod -x test_dir/
$ /usr/bin/ls test_dir/
test
$ cat test_dir/test
cat: test_dir/test: Permission denied
```

##### 3

> What permissions are required on the parent directory and the file itself 
> in order to create a new file, open a file for reading, open a file for 
> writing, and delete a file? 

To create a new file under a directory, a process must have `w` and `x` 
permissions on that directory.

To read a file, a process should have `x` on its parent directory and `r` on the 
file itself.

To write a file, a process should have `x` on its parent directory and `w` on the 
file itself.

To delete a file, a process should have `w` and `x` on its parent directory and...
Seems you don't need any permission on the file itself:

```shell
$ l -d test_dir/
Permissions Links Size User  Group Date Modified Name
drwx------@     1    - steve steve  9 Oct 14:48  test_dir
$ l test_dir/test
Permissions Links Size User  Group Date Modified Name
.r--------@     1    6 steve steve  9 Oct 14:48  test_dir/test
$ lsattr test_dir/test
---------------------- test_dir/test
$ chmod 000 test_dir/test
$ chmod u-r test_dir/
$ rm test_dir/test
rm: remove write-protected regular file 'test_dir/test'? y
```


>  What permissions are required on the source and target directory to rename
>  a file? If the target file of a rename operation already exists, what 
>  permissions are required on that file? 

To rename a file, `w` and `x` on the parent directory is sufficient.

```shell
$ l -d test_dir
Permissions Links Size User  Group Date Modified Name
d-wx------@     1    - steve steve  9 Oct 14:55  test_dir

$ touch test_dir/test
$ chmod 000 test_dir/test
$ mv test_dir/test test_dir/new_test
```
> I guess this is equivalent to `creating a new file under a directory`

```shell
$ l -d test_dir
Permissions Links Size User  Group Date Modified Name
d-wx------@     1    - steve steve  9 Oct 14:57  test_dir

$ chmod 000 test_dir/test
$ chmod 000 test_dir/new_test

$ mv test_dir/test test_dir/new_test
mv: replace 'test_dir/new_test', overriding mode 0000 (---------)? y
$
```

> I guess this is equivalent to `deleting a file`.


>  How does setting the sticky permission bit (chmod +t) of a directory affect 
>  renaming and deletion operations.

When this bit is set on a dir, unpriviledged users can unlink (unlink()/rmdir()) 
or rename (rename()) files in the directory only if they have `w + x` permisions 
on that dir **and** own either the file or directory
