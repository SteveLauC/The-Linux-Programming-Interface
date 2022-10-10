#### Question

On a system running Linux 2.6, modify the program in Listing 15-1 (t_stat.c)
so that the file timestamps are displayed with nanosecond accuracy.

#### Answer

```shell
$ uname -r
5.19.13-200.fc36.x86_64
```

My machine is running kernel 5.19.13, this doesn't matter as the `nanosecond` 
field is supported since kernel 2.6:

```
    /* Since Linux 2.6, the kernel supports nanosecond
       precision for the following timestamp fields.
       For the details before Linux 2.6, see NOTES. */

    struct timespec st_atim;  /* Time of last access */
    struct timespec st_mtim;  /* Time of last modification */
    struct timespec st_ctim;  /* Time of last status change */

#define st_atime st_atim.tv_sec      /* Backward compatibility */
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
};
```

Let's do some simple format!

```c
printf("%lld.%.9ld", (long long)ts.tv_sec, ts.tv_nsec);
```
