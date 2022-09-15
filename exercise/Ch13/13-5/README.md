#### Question

The command tail [ -n num ] file prints the last num lines (ten by default) of 
the named file. Implement this command using I/O system calls (lseek() , read(), 
write(), and so on). Keep in mind the buffering issues described in this chapter, 
in order to make the implementation efficient.

#### Verification

```shell
$ cd c-13-5
$ make
$ ./13.5 13-5.c
    assert(q.count <= n);

    node *p = q.head;
    for (int i = 0; i < q.count; i += 1) {
        printf("%s\n", p->data);
        p = p->next;
    }

    clear(&q);
}

$ valgrind --leak-check=yes --track-origins=yes ./13-5 13-5.c
...
==76152== HEAP SUMMARY:
==76152==     in use at exit: 0 bytes in 0 blocks
==76152==   total heap usage: 21 allocs, 21 frees, 1,327 bytes allocated
==76152==
==76152== All heap blocks were freed -- no leaks are possible
==76152==
==76152== For lists of detected and suppressed errors, rerun with: -s
==76152== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

#### Rust Implementation

The question asks us to use those syscalls, if we are gonna use them in Rust through
`libc`, the implementation is same as the C one.
