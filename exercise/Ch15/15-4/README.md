#### Question

The access() system call checks permissions using the process's real user and 
group IDs. Write a corresponding function that performs the checks according 
to the process's effective user and group IDs.


#### Answer

##### method 1
One way to do this is to set `RUID` to its `EUID` and call `access(2)`.

```shell
$ cd method1/c-impl
$ gcc main.c
$ touch file
$ chmod 000 file
$ ./a.out  # I don't have the read permission
a.out: main.c:10: main: Assertion `0 == my_eaccess("file", R_OK)' failed.
fish: Job 1, './a.out' terminated by signal SIGABRT (Abort)

$ sudo chown root a.out
$ sudo chmod u+s a.out
$ ./a.out  # set-UID-root process has
$
```

##### method 2

Implement `my_access` according to the `permission checking algorithm`

> `my_access()` is in `my_access.c`

There are some tests for `my_access()`, execute the compiled binary, if it exits
successfully, then tests are passed.

```shell
$ cd method2/c-impl 
$ make build && ./a.out
$
```

```shell
$ cd method2/rs-impl
$ cargo t
    Finished test [unoptimized + debuginfo] target(s) in 0.00s
     Running unittests src/main.rs (target/debug/deps/rs_impl-05c0c4ac1e348b67)

running 5 tests
test test::file_that_does_not_exist ... ok
test test::with_no_permission ... ok
test test::with_read_permission ... ok
test test::with_read_write_execute_permission ... ok
test test::with_read_write_permission ... ok

test result: ok. 5 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s
```
