# The-Linue-Programming-Interface
Src of The Linux Programming Interface and its Rust implementation (Exercise included)

#### Chapters and their corresponding directories
|Chapter|Src Directory|Exercise Directory|
|-------|-------------|------------------|
|3|[lib](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/lib)|[Ch3](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch3)|
|4|[fileio](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/fileio)|[Ch4](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch4/)|
|5|[fileio](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/fileio)|[Ch5](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch5)|
|6|[proc](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/proc)|[Ch6](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch6)|
|7|[memalloc](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/memalloc)|[Ch7](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch7)|
|8|[users_groups](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/users_groups)|[Ch8](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch8)|
|9|[proccred](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/proccred)|[Ch9](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch9)|

#### What are unimplemented/skipped in Src
|Chapter| Name| Reason| Relevant Issue|
|-------|-----|-------|---------------|
|6|[mem_segments.rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/mem_segments.c)|Rust does not allow global variables to be unitialized, thus we can not allocate variable in .bss segment|None|
|6|[longjmp.rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/longjmp.c)|Relevant syscalls are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|[#1](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/1)|
|6|[setjmp_vars.rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/setjmp_vars.c)|Relevant syscalls are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|[#1](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/1)|
|8|[check_passwords-rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/users_groups/check_password-c)|`crypt(3)` is not exposed in `libc`|[#3](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/3)|


#### What are unimplemented/skipped in Exercise
|Chapter| Exercise| Reason|Relevant Issue|
|-------|----------|------|--------------|
|4|[4-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch4/4-2)|Currently I am not sure about what is a hole and how to create it, for more info, you can check [this post](https://github.com/SteveLauC/blog/issues/1)|None|
|6|[6-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch6/6-2/6-2.c)|`setjmp` and `longjmp` are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|[#1](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/1)|
|6|[6-3](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch6/6-3/6-3.c)|We can [not](https://github.com/rust-lang/libc/issues/2520) directly access `environ` in Rust whereas this is necessary to unset the environment variables that are defined multiple times|[#2](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/2)|

#### What can be improved in Exercise

|Chapter|Exercise|Description|Relevant Issue|
|-------|--------|-----------|--------------|
|7|[7-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch7/7-2)|The implementation of `free()` can be improved by merging memory fragments|None|
