use std::env::args;
use std::fs::{File, OpenOptions};
use std::io::{Seek, SeekFrom, Write};
use std::path::Path;
use std::process::exit;

fn main() {
    let av: Vec<String> = args().collect();
    if av.len() != 2 {
        eprintln!("usage: {} pathname", av[0]);
        exit(1);
    }
    let p: &Path = Path::new(av[1].as_str());
    if !p.exists() || p.metadata().unwrap().len() == 0 {
        eprintln!("path must be a non-empty valid file");
    }

    let mut f: File = OpenOptions::new()
        .write(true)
        .append(true)
        .open(&av[1])
        .expect("can not open or create");

    f.seek(SeekFrom::Start(0)).expect("can not seek");

    f.write("test".as_bytes()).expect("can not write");
}
