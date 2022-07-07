//! usage: seek_io file {options}
//!
//! options:
//!     soffset: seek to byte offset from the start of the file
//!     rlength: read length bytes from the file, starting at the current
//!         file offset, and display them in text form
//!     Rlength: read length bytes from the file, starting at the current
//!         file offset, and display them in hexadecimal form
//!     wstr: write the string of characters specified in str at the current
//!         file offset

use rust_lib::get_num::{getLong, GnFlag};
use std::env::{args, Args};
use std::fs::{File, OpenOptions};
use std::io::{Read, Seek, SeekFrom, Write};
use std::os::unix::prelude::OpenOptionsExt;

fn main() {
    let mut av: Args = args();
    if av.len() < 3 {
        eprintln!("usage: seek_io file r<length>|R<length>|w<string>|s<offset>...");
    }

    let mut file: File = OpenOptions::new()
        .write(true)
        .read(true)
        .create(true)
        .mode(0o666)
        .open(av.nth(1).unwrap())
        .expect("can not open file");

    av.for_each(|opt: String| {
        if opt.starts_with('r') || opt.starts_with('R') {
            let len: i64 = getLong(Some(&opt[1..]), GnFlag::GN_ANY_BASE, Some(&opt));
            let mut buf: Vec<u8> = vec![0; usize::try_from(len).unwrap()];
            file.read_exact(&mut buf).expect("can not read");
            println!("{}", &opt);
            for byte in buf.iter() {
                if opt.starts_with('r') {
                    print!("{} ", char::from(*byte));
                } else {
                    print!("{:02x} ", byte);
                }
            }
            println!();
        } else if let Some(str_without_prefix) = opt.strip_prefix('w') {
            file.write_all(str_without_prefix.as_bytes())
                .expect("can not write");
            println!("seek_io: write {} bytes", &opt[1..].len());
        } else if let Some(str_without_prefix) = opt.strip_prefix('s') {
            file.seek(SeekFrom::Start(
                u64::try_from(getLong(
                    Some(str_without_prefix),
                    GnFlag::GN_ANY_BASE,
                    Some(&opt),
                ))
                .unwrap(),
            ))
            .expect("can not seek");
            println!("seek_io: seek succeeded");
        } else {
            eprintln!("invalid option");
        }
    })
}
