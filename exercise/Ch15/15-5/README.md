#### Question

As noted in Section 15.4.6, umask() always sets the process umask and, at the 
same time, returns a copy of the old umask. How can we obtain a copy of the 
current process umask while leaving it unchanged?


##### Answer

We can call it twice, like this:

```c
mode_t fetch_umask(void) {
     mode_t prev = umask(0);
     umask(prev);
     return prev;
}
```

But this is not thread-safe.

There is a [GNU extension function](https://man7.org/linux/man-pages/man3/getumask.3.html) 
tries to provide this functionality in a thread-safe way:

```c
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>

mode_t getumask(void);
```

Though there in no wrapper in current glibc.
