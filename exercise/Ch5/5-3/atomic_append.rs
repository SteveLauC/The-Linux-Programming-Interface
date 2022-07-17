use std::env::args;
use std::fs::{File, OpenOptions};
use std::io::{Seek, SeekFrom, Write};
use std::os::unix::fs::OpenOptionsExt;
use std::process::exit;

fn main() {
    let av: Vec<String> = args().collect();
    if av.len() < 3 || av.len() > 4 {
        eprintln!("usage: {} filename num-bytes [x]", av[0]);
        exit(1);
    }

    let mut f: File = OpenOptions::new()
        .write(true)
        .create(true)
        .mode(0o666)
        .append(av.len() == 3)
        .open(&av[1])
        .expect("open");

    let num_bytes: i32 = av[2].parse().expect("not a valid number");

    for _ in 0..num_bytes {
        if av.len() == 4 {
            f.seek(SeekFrom::End(0)).expect("can not seek");
        }
        f.write("h".as_bytes()).expect("can not write");
    }
}
