mod vector_io;
use vector_io::{VectorInput, VectorOutput};

use std::{
    fs::{File, OpenOptions},
    io::stdout,
};

fn main() {
    let mut buf1: [u8; 10] = [0; 10];
    let mut buf2: [u8; 10] = [0; 10];

    let mut source: File = OpenOptions::new().read(true).open("source").expect("open");

    let num_read: usize = source.read_v(&mut [&mut buf1, &mut buf2]).expect("read_v");

    println!("request 20 bytes, read {} bytes", num_read);

    let num_written: usize = stdout().write_v(&[&buf1, &buf2]).expect("write_v");
    println!("write {} bytes to stdout", num_written);
}
