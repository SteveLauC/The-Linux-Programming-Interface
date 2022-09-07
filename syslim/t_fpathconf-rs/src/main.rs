use nix::{
    libc::STDIN_FILENO,
    unistd::{fpathconf, PathconfVar},
};
use std::{os::unix::io::RawFd, process::exit};

fn main() {
    fpathconf_print("_PC_NAME_MAX", STDIN_FILENO, PathconfVar::NAME_MAX);
    fpathconf_print("_PC_PATH_MAX", STDIN_FILENO, PathconfVar::PATH_MAX);
    fpathconf_print("_PC_PIPE_BUF", STDIN_FILENO, PathconfVar::PIPE_BUF);
}

fn fpathconf_print(msg: &str, fd: RawFd, name: PathconfVar) {
    match fpathconf(fd, name) {
        Ok(Some(val)) => println!("{} {}", msg, val),
        Ok(None) => println!("{} indeterminate", msg),
        Err(_) => {
            println!("fpathconf {}", msg);
            exit(1);
        }
    }
}
