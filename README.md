# The-Linue-Programming-Interface
Src of The Linux Programming Interface and its Rust implementation (Exercise included)

#### Chapters and their corresponding directories
|Chapter|Src Directory|Exercise Directory|
|-------|-------------|------------------|
|3 System Programming Concepts|[lib](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/lib)|[Ch3](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch3)|
|4 File I/O: The Universal I/O Model|[fileio](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/fileio)|[Ch4](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch4/)|
|5 File I/O: Further Details|[fileio](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/fileio)|[Ch5](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch5)|
|6 Processes|[proc](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/proc)|[Ch6](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch6)|
|7 Memory Allocation|[memalloc](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/memalloc)|[Ch7](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch7)|
|8 Users and Groups|[users_groups](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/users_groups)|[Ch8](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch8)|
|9 Process Credentials|[proccred](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/proccred)|[Ch9](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch9)|
|10 Time|[time](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/time)|[Ch10](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch10)|
|11 System Limits And Options|[syslim](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/syslim)|[Ch11](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch11)|
|12 System And Process Information|[sysinfo](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/sysinfo)|[Ch12](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch12)|
|13 File I/O Buffering|[filebuff](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/filebuff)|[Ch13](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch13)|
|14 File Systems|[filesys](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/filesys)|[Ch14](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch14)|
|15 File Attributes|[files](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/files)|[Ch15](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch15)|
|16 Extended Attributes|[xattr](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/xattr)|[Ch16](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch16)|
|17 Access Control Lists |[acl](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/acl)|[Ch17](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch17)|
|18 Directories and Links|[dirs_links](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/dirs_links)|[Ch18](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch18)|
|19 Monitoring File Events|[inotify](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/inotify)|[Ch19](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch19)|
|20 Signals: Fundamental Concepts|[inotify](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/signals)|[Ch20](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch20)|


#### What are unimplemented/skipped in Src
|Chapter| Name| Reason| Relevant Issue|
|-------|-----|-------|---------------|
|6|[mem_segments.rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/mem_segments.c)|Rust does not allow global variables to be unitialized, thus we can not allocate variable in .bss segment|None|
|6|[longjmp.rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/longjmp.c)|Relevant syscalls are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|[#1](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/1)|
|6|[setjmp_vars.rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/proc/setjmp_vars.c)|Relevant syscalls are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|[#1](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/1)|
|8|[check_passwords-rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/users_groups/check_password-c)|`crypt(3)` is not exposed in `libc`|[#3](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/3)|
|10|[calendar_time-rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/time/calendar_time.c)| `ctime(3)` and `asctime(3)` are not exposed in `libc`| [#4](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/4)|
|10|[strtime-rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/time/strtime.c)|`strftime(3)` and `strptime(3)` are not exposed in `libc` | [#5](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/5)|
|10|[showtime-rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/time/show_time.c)|`strftime(3)` and `asctime(3)` are not exposed in `libc` | None |
|10|[process_time-rs](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/time/process_time.c)|`clock(3)` is not exposed in `libc` | None |


#### What are unimplemented/skipped in Exercise
|Chapter| Exercise| language|Reason|Relevant Issue|
|-------|---------|---------|------|--------------|
|4|[4-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch4/4-2)|C and Rust|Currently I am not sure about what is a hole and how to create it, for more info, you can check [this post](https://github.com/SteveLauC/blog/issues/1)|None|
|6|[6-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch6/6-2)|Rust|`setjmp` and `longjmp` are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|[#1](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/1)|
|6|[6-3](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch6/6-3)|Rust|We can [not](https://github.com/rust-lang/libc/issues/2520) directly access `environ` in Rust whereas this is necessary to unset the environment variables that are defined multiple times|[#2](https://github.com/SteveLauC/The-Linux-Programming-Interface/issues/2)|
|15|[15-7](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch15/15-7)|Rust|`FS_*` constants are missing from `libc`|None|
|17|[17-1](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch17/17-1)|C|I don' want to touch those `acl_xxx_xxx()` functions|None|

#### What can be improved in Exercise

|Chapter|Exercise|Description|Relevant Issue|
|-------|--------|-----------|--------------|
|7|[7-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch7/7-2)|The implementation of `free()` can be improved by merging memory fragments|None|
