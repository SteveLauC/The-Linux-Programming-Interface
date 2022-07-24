## Process

#### What is unimplemented and why?
1. [mem_segments.c](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/mem_segments.c)
   
   usage: to demonstrate memory locations of different types of variables

   reason: Rust does not allow global variables to be unitialized, thus we can not 
   allocate variable in .bss segment

2. [longjmp.c](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/longjmp.c)

   usage: to demonstrate the usage of non-local functions `setjmp` and `longjmp`

   reason: these two syscalls are missing in `libc`
