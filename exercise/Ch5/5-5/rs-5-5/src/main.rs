mod dup;
use dup::{Dup, StatusFlag};
use nix::fcntl::OFlag;
use std::{
    fs::{File, OpenOptions},
    io::{Seek, SeekFrom, Write},
    os::unix::prelude::OpenOptionsExt,
};

fn main() {
    let mut file1: File = OpenOptions::new()
        .create(true)
        .write(true)
        .mode(0o666)
        .open("tmp")
        .expect("open");
    let mut file2: File = file1.dup().expect("dup");

    let mut offset1 = file1.seek(SeekFrom::Current(0)).expect("seek");
    let mut offset2 = file2.seek(SeekFrom::Current(0)).expect("seek");

    println!(
        "before modification: offset1: {} | offset2: {}",
        offset1, offset2
    );

    println!("writing 5 bytes to fd1...");

    file1.write("hello".as_bytes()).expect("write");
    offset1 = file1.seek(SeekFrom::Current(0)).expect("seek");
    offset2 = file2.seek(SeekFrom::Current(0)).expect("seek");

    println!(
        "after modification: offset1: {} | offset2: {}\n",
        offset1, offset2
    );

    let mut flag1: OFlag = file1.get().expect("get");
    let mut flag2: OFlag = file2.get().expect("get");

    println!(
        "before modification: flag1 has O_NONBOCK {}| flag2 has O_NONBLOCK {}",
        flag1.contains(OFlag::O_NONBLOCK),
        flag2.contains(OFlag::O_NONBLOCK)
    );

    println!("setting O_NONBLOCK flag to fd1...");
    file1.set(OFlag::O_NONBLOCK).expect("set");

    flag1 = file1.get().expect("get");
    flag2 = file2.get().expect("get");

    println!(
        "after modification: flag1 has O_NONBOCK {}| flag2 has O_NONBLOCK {}",
        flag1.contains(OFlag::O_NONBLOCK),
        flag2.contains(OFlag::O_NONBLOCK)
    );
}
