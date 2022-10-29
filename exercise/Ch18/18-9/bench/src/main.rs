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
