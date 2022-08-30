#### Question
       
Implement `int initgroups(const char *user, gid_t group)` using `int getgroups(int size,
gid_t list[])` and library functions for retrieving information from the password
and group files (section 8.4). Remeber a process must be priviledged in order to
be able to call `setgroups()`.

#### Implementation

1. Find out the groups to which `user` belongs.
2. Allocate an array to store the GIDs gotten in step 1 plus `group`.
3. call `setgroup(array_len, array)` (Need priviledge)

##### How to get the groups to which `user` belongs

1. Use `getpwnam_r` to find the first group that `user` is in.
2. Use `getpwent_r` to sequentially scan `/etc/group` to find the remaining groups

#### Verification

```shell
$ gcc 9-3.c -0 9-3
$ sudo ./9-3
Supplementary GIDs of current process: 10 999 1000
```
