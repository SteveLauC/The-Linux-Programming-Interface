use libc::O_DIRECT;
use std::{
    alloc::{alloc, Layout},
    env::{args, Args},
    fs::{File, OpenOptions},
    io::{Seek, SeekFrom, Read},
    os::unix::fs::OpenOptionsExt,
    process::exit,
    slice::from_raw_parts_mut,
};

fn arg_init() -> (String, usize, usize, usize) {
    let mut av: Args = args();
    let file_name: String = if let Some(file_name) = av.nth(1) {
        file_name
    } else {
        eprintln!("usage: direct_read FILE NUM_BYTES_TO_READ [OFFSET] [BUFF_ALIGNMENT]");
        exit(1);
    };
    let num_bytes_to_read: usize = if let Some(num) = av.nth(0) {
        num.parse().expect("not a valid number")
    } else {
        eprintln!("usage: direct_read FILE NUM_BYTES_TO_READ [OFFSET] [BUFF_ALIGNMENT]");
        exit(1);
    };

    let offset: usize = if let Some(off) = av.nth(0) {
        off.parse().expect("not a valid offset")
    } else {
        0
    };
    let alignment: usize = if let Some(align) = av.nth(0) {
        align.parse().expect("not a valid alignment")
    } else {
        4096
    };

    (file_name, num_bytes_to_read, offset, alignment)
}

fn main() {
    let (file_name, num_bytes_to_read, offset, alignment) = arg_init();

    let mut file: File = OpenOptions::new()
        .read(true)
        .custom_flags(O_DIRECT)
        .open(file_name)
        .expect("open");

    let ptr: *mut u8 =
        unsafe { alloc(Layout::from_size_align(num_bytes_to_read, alignment).unwrap()) };
    let slice: &mut [u8] = unsafe { from_raw_parts_mut(ptr, num_bytes_to_read) };

    file.seek(SeekFrom::Start(u64::try_from(offset).unwrap()))
        .expect("can not seek");

    match file.read(slice ) {
        Ok(num_read) => println!("Read {} bytes", num_read),
        Err(error) => {
            eprintln!("{}", error);
            exit(1);
        }
    }
}
