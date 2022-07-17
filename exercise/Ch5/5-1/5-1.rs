//! The file size and offset used by Rust on i686-linux are both 64 bits long, so
//! we don't have such problems.

use std::env::args;
use std::fs::{File, OpenOptions};
use std::io::{Seek, SeekFrom, Write};
use std::os::unix::fs::OpenOptionsExt;
use std::process::exit;

fn main() {
    let av: Vec<String> = args().collect();
    if av.len() != 3 {
        eprintln!("usage: {} pathname offset", av[0]);
        exit(1);
    }

    let mut f: File = OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .mode(0o600)
        .open(&av[1])
        .expect("can not open or create");

    f.seek(SeekFrom::Current(
        av[2].parse().expect("not a valid number"),
    ))
    .expect("can not seek");

    f.write("test".as_bytes()).expect("can not write");
}
