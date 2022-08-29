#### Question
Assume in each of the following cases that the initial set of process user IDs
is `real = 1000 effective = 0 saved = 0 file-system = 0`. What would be the state
of the user IDs after the following calls.

1. setuid(2000)
2. setreuid(-1, 2000)
3. seteuid(2000)
4. setfsuid(2000)
5. setresuid(-1, 2000, 3000);

#### Answers
1. real=2000 eff=2000 saved=2000 fs=2000
   
   Process is priviledged, then `RUID/EUID/SUID/fs-UID` will always have the same
   value `2000`

2. real=1000 eff=2000 saved=2000 fs=2000
   
   `RUID` remains unchanged, `EUID` will be set to `2000`. Due to fact that new 
   `EUID` does not equal to `RUID`, so the `SUID` will also be set. `fs-UID` normally
   always have the same value as `EUID`

3. real=1000 eff=2000 saved=0 fs=2000
   
   Only `EUID` and `fs-UID` will be changed.

4. real=1000 eff=0 saved=0 fs=2000

   Only `fs-UID` will be changed.

5. real=1000 eff=2000 saved=3000 fs=2000

   `RUID` remains unchanged, `EUID` and `SUID` will be set to `2000` and `3000`
   individually. `fs-UID` normally always have the same value with `EUID`, which
   is `2000`

#### Verification
This process is obviously a `set-UID-root program`

```rust
use nix::{
    libc::{setreuid, uid_t},
    unistd::{seteuid, setfsuid, setresuid, setuid, Uid},
};
use std::{
    env::args,
    fs::{File, OpenOptions},
    io::{BufRead, BufReader},
    process::exit,
};

fn main() {
    let av: Vec<String> = args().collect();
    if av.len() != 2 {
        exit(1);
    }

    match av[1].as_str() {
        "1" => setuid(Uid::from_raw(2000)).unwrap(),
        "2" => unsafe {
            assert_eq!(setreuid(uid_t::MAX, 2000), 0);
        },
        "3" => seteuid(Uid::from_raw(2000)).map(drop).unwrap(),
        "4" => {
            setfsuid(Uid::from_raw(2000));
        }
        "5" => setresuid(
            Uid::from_raw(uid_t::MAX),
            Uid::from_raw(2000),
            Uid::from_raw(3000),
        )
        .unwrap(),
        _ => unreachable!(),
    };

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
```
```shell
$ cargo b -q
$ cd target/debug
$ sudo chown root rust
$ sudo chmod u+s rust
```

```shell
$ ./rust 1
Uid:    2000    2000    2000    2000

$ ./rust 2
Uid:    1000    2000    2000    2000

$ ./rust 3
Uid:    1000    2000    0       2000

$ ./rust 4
Uid:    1000    0       0       2000

$ ./rust 5
Uid:    1000    2000    3000    2000
```
