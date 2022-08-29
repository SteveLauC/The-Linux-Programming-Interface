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
