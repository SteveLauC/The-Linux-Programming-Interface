use nix::errno::Errno;
use nix::fcntl::{fcntl, open, FcntlArg, OFlag};
use nix::sys::stat::Mode;
use nix::unistd::close;
use nix::Result;
use std::env::args;
use std::os::unix::io::RawFd;
use std::process::exit;

fn main() {
    let av: Vec<String> = args().collect();
    if av.len() != 3 {
        eprintln!("usage: {} file1 file2", av[0]);
        exit(1);
    }

    let file1: RawFd = open(
        av[1].as_str(),
        OFlag::O_CREAT | OFlag::O_WRONLY,
        Mode::from_bits(0o666).unwrap(),
    )
    .expect("open");
    close(1).unwrap();
    my_dup(file1).unwrap();
    println!("hello stdout");

    let file2: RawFd = open(
        av[2].as_str(),
        OFlag::O_CREAT | OFlag::O_WRONLY,
        Mode::from_bits(0o666).unwrap(),
    )
    .expect("open");
    my_dup2(file2, 2).unwrap();
    eprintln!("hello stderr");

    close(file1).unwrap();
    close(file2).unwrap();
}

fn my_dup(oldfd: RawFd) -> Result<RawFd> {
    Errno::clear();
    if let Err(Errno::EBADF) = fcntl(oldfd, FcntlArg::F_GETFL) {
        return Err(Errno::EBADF);
    }

    fcntl(oldfd, FcntlArg::F_DUPFD(0))
}

fn my_dup2(oldfd: RawFd, newfd: RawFd) -> Result<RawFd> {
    Errno::clear();
    if let Err(Errno::EBADF) = fcntl(oldfd, FcntlArg::F_GETFL) {
        return Err(Errno::EBADF);
    }

    if oldfd == newfd {
        return Ok(newfd);
    }

    if fcntl(newfd, FcntlArg::F_GETFL).is_ok() {
        close(newfd).unwrap();
    }

    match fcntl(oldfd, FcntlArg::F_DUPFD(newfd)) {
        Ok(new_open_fd) => {
            if new_open_fd == newfd {
                return Ok(newfd);
            } else {
                close(new_open_fd).unwrap();
                return Err(Errno::last());
            }
        }
        _ => {
            return Err(Errno::last());
        }
    }
}
