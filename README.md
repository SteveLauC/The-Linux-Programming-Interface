# The-Linue-Programming-Interface
Source code of The Linux Programming Interface.

#### Chapters and their corresponding directories
|Chapter|Src Directory|Exercise Directory|
|-------|-------------|------------------|
|3|[lib](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/lib)|[Ch3](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch3)|
|4|[fileio](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/fileio)|[Ch4](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch4/)|
|5|[fileio](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/fileio)|[Ch5](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch5)|
|6|[proc](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/proc)|[Ch6](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch6)|
|7|[memalloc](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/memalloc)|[Ch7](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch7)|

#### What are unimplemented/skipped in Src
|Chapter| Name| Reason| Relevant Issue|
|-------|-----|-------|---------------|
|6|[mem_segments.c](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/mem_segments.c)|Rust does not allow global variables to be unitialized, thus we can not allocate variable in .bss segment|None|
|6|[longjmp.c](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/longjmp.c)|Relevant syscalls are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|[#1](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/1)|
|6|[setjmp_vars.c](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/setjmp_vars.c)|Relevant syscalls are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|[#1](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/1)|


#### What are unimplemented/skipped in Exercise
|Chapter| Exercise| Reason|Relevant Issue|
|-------|----------|------|--------------|
|4|4-2|Currently I am not sure about what is a hole and how to create it, for more info, you can check [this post](https://github.com/SteveLauC/blog/issues/1)|None|
|6|[6-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch6/6-2/6-2.c)|`setjmp` and `longjmp` are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|[#1](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/1)|
|6|[6-3](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch6/6-3/6-3.c)|We can [not](https://github.com/rust-lang/libc/issues/2520) directly access `environ` in Rust whereas this is necessary to unset the environment variables that are defined multiple times|[#2](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/2)|

#### What can be improved in Exercise

|Chapter|Exercise|Description|Relevant Issue|
|-------|--------|-----------|--------------|
|7|[7-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch7/7-2)|The implementation of `free()` can be improved by merging memory fragments|None|
