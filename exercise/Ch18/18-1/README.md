#### Question

In Section 4.3.2, we noted that it is not possible to open a file for writing
if it is currently being executed (open() returns -1, with `errno` set to `ETXTBSY`).
Nevertheless, it is possible to do the following from the shell:

```shell
$ cc -o longrunner longrunner.c
$ ./longrunner &   # Leave running in background
$ vi longrunner.c  # Make some changes to the source code
$ cc -o longrunner longrunner.c
```

The last command **overwrites** the existing executable of the same name. How is this
possible? (For a clue, use `ls -li` to look at the i-node number of the executable
file after each compilation.)

#### Verification

```rust
// longrunner.rs

use std::{
    thread::sleep,
    time::Duration,
};

fn main() {
    sleep(Duration::from_secs(1000_000));
}
```

```shell
$ rustc longrunner.rs 
$ l -i longrunner
  inode Permissions Links Size User  Group Date Modified Name
4586779 .rwxr-xr-x@     1 3.9M steve steve 24 Oct 08:27  longrunner
$ ./longrunner &

$ echo "hello" > longrunner
warning: An error occurred while redirecting file 'longrunner'
open: Text file busy

$ vim longrunner.rs
$ rustc longrunner.rs

$ l -i longrunner
  inode Permissions Links Size User  Group Date Modified Name
4586825 .rwxr-xr-x@     1 3.9M steve steve 24 Oct 08:29  longrunner

$ echo "hello" > longrunner
$
```

As you can see, they have different i-node numbers, which means they are not the
same file. The second compilation didn't **modify** the first `longrunner`, but
replaced it.
