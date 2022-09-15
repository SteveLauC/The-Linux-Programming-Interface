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
```

#### Memory issue

The current implementation does not leak memory, but has some memory access 
issues.

```shell
$ valgrind --leak-check=yes --track-origins=yes ./13-5 13-5.c
...
==70626== HEAP SUMMARY:
==70626==     in use at exit: 0 bytes in 0 blocks
==70626==   total heap usage: 21 allocs, 21 frees, 1,327 bytes allocated
==70626==
==70626== All heap blocks were freed -- no leaks are possible
==70626==
==70626== For lists of detected and suppressed errors, rerun with: -s
==70626== ERROR SUMMARY: 10 errors from 1 contexts (suppressed: 0 from 0)
```

#### Rust Implementation

The question asks us to use those syscalls, if we are gonna use them in Rust through
`libc`, the implementation is same as the C one.
