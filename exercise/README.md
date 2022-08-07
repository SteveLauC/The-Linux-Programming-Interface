## My solution to The Linux Programming Interface.

#### What are unimplemented/skipped
|Chapter| Exercise| Reason|
|-------|----------|------|
|4|4-2|Currently I am not sure about what is a hole and how to create it, for more info, you can check [this post](https://github.com/SteveLauC/blog/issues/1)|
|6|[6-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch6/6-2/6-2.c)|`setjmp` and `longjmp` are [missing](https://github.com/rust-lang/libc/issues/1208) in `libc`|
|6|[6-3](https://github.com/SteveLauC/The-Linux-Programming-Interface/blob/main/exercise/Ch6/6-3/6-3.c)|We can [not](https://github.com/rust-lang/libc/issues/2520) directly access `environ` in Rust whereas this is necessary to unset the environment variables that are defined multiple times|

#### What can be improved

|Chapter|Exercise|Description|
|-------|--------|-----------|
|7|[7-2](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch7/7-2)|The implementation of `free()` can be improved by merging memory fragments|
