#### QUESTION

compile the program in listing 6-1 (mem_segments.c), and list its size 
using ls -l. Although the program contains an array (mbuf) that is around 10 MB
in size, the executable file is much smaller than this. Why is this?

#### ANSWER

`mbuf` is a static uninitialized variable, which will be loaded into the
`.bss` section when executed. All things that will go into this section will be
set to 0 at runtime, so there is no need to store these 0s (empty bytes) in ELF.

```shell
$ gcc mem_segments.c ../lib/error_functions.c -o mem_segments

$ file mem_segments
mem_segments: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=832a6ab1736ea9e00de4c503b4ea49bc8eafb865, for GNU/Linux 3.2.0, not stripped

$ readelf -a mem_segments > readelf_results
```

If we use `readelf` to check it out, look at the headers of type `LOAD` (this type of header
will be loaded into memory when executed)

```
LOAD           0x0000000000000000 0x0000000000000000 0x0000000000000000
               0x0000000000001670 0x0000000000001670  R      0x1000
LOAD           0x0000000000002000 0x0000000000002000 0x0000000000002000
               0x0000000000000bf9 0x0000000000000bf9  R E    0x1000
LOAD           0x0000000000003000 0x0000000000003000 0x0000000000003000
               0x0000000000000770 0x0000000000000770  R      0x1000
LOAD           0x0000000000003d48 0x0000000000004d48 0x0000000000004d48
               0x0000000000000728(filesize) 0x00000000009d4778(memsize)  RW     0x1000
```

You will find the last one's filesize and memsize differ enormously (filesize is
the size it takes in ELF file, memsize is the size it is gonna occupy when loaded
into memory), and this is where our `mbuf` belongs to.
