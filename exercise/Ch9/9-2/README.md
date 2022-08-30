#### Question
Is a process with the following user IDs priviledged? Explain your answer.

```
real=0 eff=1000 saved=1000 fs=1000
```

This is a `set-UID` process whose owner's `uid` is `1000`, and is executed by 
root.
With `EUID` not equal to `0`, this is obviously a `unpriviledged` process. But
it can easily obtain priviledge through `seteuid(getuid())`.

> Since kernel 2.2, Linux introduces `capicilities`, which are smaller units
> that can be individually enabled to gain the priviledge. This question does
> not mention it, so I ignore this.

#### Verification

A unpriviledged can change its `EUID` to only `current RUID` and `current saved
set-UID`, we will try to execute `seteuid(3000)` to see if we can get `EPERM` 
error

```rust
use nix::{
    errno::Errno,
    unistd::{seteuid, Uid},
};
use std::{
    fs::{File, OpenOptions},
    io::{BufRead, BufReader},
};

fn print_uids() {
    let status: BufReader<File> = BufReader::new(
        OpenOptions::new()
            .read(true)
            .open("/proc/self/status")
            .unwrap(),
    );
    for opt_line in status.lines() {
        if let Ok(line) = opt_line {
            if line.starts_with("Uid") {
                println!("{}", line);
                break;
            }
        }
    }
}

fn main() {
    print_uids();
    match seteuid(Uid::from_raw(3000)) {
        Ok(_) => println!("Priviledged"),
        Err(Errno::EPERM) => println!("Unpriviledged"),
        _ => unreachable!(),
    }
}
```
```shell
$ cargo b -q
$ cd target/debug
$ chmod u+s verify
$ l verify
Permissions Size User  Group Date Modified Name
.rwsr-xr-x@ 6.4M steve steve 30 Aug 07:44  verify
$ sudo ./verify
Uid:    0       1000    1000    1000
Unpriviledged
```
