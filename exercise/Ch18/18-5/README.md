#### Question

Implement a function that performs the equivalent of `getcwd()`. A useful tip
for solving this problem is that you can find the name of the current working
directory by using `opendir()` and `readdir()` to walk through each of the 
entries in the parent directory (..) to find an entry with the same `i-node`
and device number as the current working directory (i.e., respectively, the
`st_ino` and `st_dev` fields in the `stat` structure retunrned by `stat()` and
`lstat()`). Thus, it is possible to construct the directory path by walking
up the directory ree (`chdir("..")`) one step at a time and performing such
scans. The walk can be finished when the parent directory is the same as the
the current walking direcotry (recall that `/..` is the same as `/`). The caller
should be left in the same directory in which it started, regardless of whether
your `getcwd()` function succeeds or fails (`open()` plus `fchdir()` are handy
for this purpose).

#### Answer

```shell
$ cd c-impl
$ make
$ ./getcwd
/home/steve/Documents/workspace/The-Linux-Programming-Interface/exercise/Ch18/18-5/c-impl
```
