#### Question

In section 18.10, we showed two different techniques (using `fchdir(2)` and 
`chdir(2)`, respectively) to return the previous current working directory
after changing the operation repeatedly. Which method do you expect to be
more efficient? Why? Write a program to confirm your answer.

#### My Guess

According to this [post](https://stackoverflow.com/a/3781614/14092446), a 
process is represented as [`struct task_struct`](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/sched.h?id=HEAD#n737).
And `task_struct` has  a pointer to [`fs_struct`](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/fs_struct.h?id=HEAD#n9).
`CWD` and `root` are stored in fiedls `cwd` and `root`, which are of type
[`struct path`](https://elixir.bootlin.com/linux/latest/source/include/linux/path.h).
`struct path` is basically a pair of `vfsmount` and `dentry`, the former 
represents a file system, the latter has a pointer to `struct inode`, which
uniquely locates a file within a specific file system. So `struct path` can 
locate a file.

File descriptor is "closer" to `i-node` when compared to `file name` (don't need
an extra `open()` syscall)

https://github.com/SteveLauC/pic/blob/main/relation_between_fd_and_open_files.jpeg

So I guess `fchdir(2)` is faster.


#### Verification

```rust
use nix::{
    fcntl::{open, OFlag},
    sys::stat::Mode,
    unistd::{chdir, close, fchdir},
};
use std::time::SystemTime;

const TARGET_CWD: [&str; 5] = ["/", "/home", "/usr", "/dev", "/proc"];

fn chdir_bench() {
    let start = SystemTime::now();

    for cwd in TARGET_CWD {
        assert!(chdir(cwd).is_ok());
    }

    let end = SystemTime::now();
    let duration = end.duration_since(start).unwrap();
    println!("chdir takes: {:?}", duration);
}

fn fchdir_bench() {
    let mut fds = [0; 5];
    for idx in 0..TARGET_CWD.len() {
        let fd = open(TARGET_CWD[idx], OFlag::O_RDONLY, Mode::empty()).unwrap();
        fds[idx] = fd;
    }

    let start = SystemTime::now();

    for fd in fds {
        assert!(fchdir(fd).is_ok());
    }

    let end = SystemTime::now();
    let duration = end.duration_since(start).unwrap();
    println!("fchdir takes: {:?}", duration);

    for fd in fds {
        close(fd).unwrap();
    }
}

fn main() {
    chdir_bench();
    fchdir_bench();
}
```

```shell
$ cd bench
$ cargo r -q
chdir takes: 6.562µs
fchdir takes: 1.382µs
```

A `open()` syscall will roughly take:

```rs
use std::time::Instant;
use nix::fcntl::{OFlag, open};
use nix::sys::stat::Mode;

fn main() {
    let now = Instant::now();
    let _ = open("Cargo.toml", OFlag::O_RDONLY, Mode::empty()).unwrap();

    println!("{:?}", now.elapsed());
}
```

```sh
$ cargo b -r
$ ./target/release/rust
3.046µs
```

So `chdir()` is equivalent to `open()` + `fchdir()`?
