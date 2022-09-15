#### Question

What is the effect of the following statements?

```c
fflush(fp);
fsync(fileno(fp));
```

If `fp` refers to an INPUT stream, the contents of library buffer are 
discarded. Then it is guaranteed that the requested data has been transferred 
from the disk to the process. If a pending write operation will affect the 
requested data, it is performed before the read operation according to the requirements 
of synchronized I/O data integrity completion.

If `fp` refers to a OUTPUT stream, then the libray buffer is flushed to
the kernel buffer cache. Then `fsync()` will write all data and metadata from
the kernel buffer cache to the disk (at least its cache), making this write 
operation reach `synchronized I/O file integrity completion`.
