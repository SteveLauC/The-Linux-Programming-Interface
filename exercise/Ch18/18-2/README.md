#### Question

Why does the call to `chmod()` in the following code fail?

```c
mkdir("test", S_IRUSR|S_IWUSR|S_IXUSR); // 0700
chdir("test");
fd = open("myfile", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR); // 0600
symlink("myfile", "../mylink");
chmod("../mylink", S_IRUSR);
```

#### Answer

`chmod()` failed because `mylink` **is a dangling symlink**. This is because
symlink can be either `relative` and `absolute`, if it is a relative path,
then it is relative to the link itself. 

The content of `mylink` is `myfile`, then this symlink points to `./myfile`,
which does not exist.

We simulate this C snippet using shell commands:

```shell
$ mkdir test
$ chmod 700 test
$ cd test
$ touch myfile
$ chmod 600 myfile
$ ln -s myfile ../mylink
$ readlink ../mylink
myfile

$ cd ..
$ l mylink
Permissions Links Size User  Group Date Modified Name
lrwxrwxrwx      1    6 steve steve 24 Oct 08:43  mylink -> myfile

$ l myfile
"myfile": No such file or directory (os error 2)
```

A correct command to create `mylink` would be `ln -s test/myfile ../mylink`.
