## My solution to The Linux Programming Interface.

#### What is unimplemented/skipped
|Chapter| Exercise| Reason|
|-------|----------|------|
|4|4-2|Currently I am not sure about what is a hole and how to create it, for more info, you can check [this post](https://github.com/SteveLauC/blog/issues/1)|
|6|6-2|`setjmp` and `longjmp` are missing in `libc`|
|6|6-3|We can not directly access `environ` in Rust whereas this is necessary to unset the environment variables that are defined multiple times|
