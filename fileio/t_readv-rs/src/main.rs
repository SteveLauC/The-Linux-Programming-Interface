use nix::sys::stat::FileStat;
use std::env::args;
use std::fs::File;
use std::io::{IoSliceMut, Read};
use std::mem::{size_of_val, MaybeUninit};
use std::process::exit;
use std::slice::from_raw_parts_mut;

const STR_SIZE: usize = 100;
fn main() {
    let av: Vec<String> = args().collect();
    if av.len() != 2 {
        eprintln!("usage: {} file", av[0]);
        exit(1);
    }

    let mut my_struct: FileStat = unsafe { MaybeUninit::zeroed().assume_init() };
    let mut x: i32 = 0;
    let mut str: [u8; STR_SIZE] = [0; STR_SIZE];

    let buffer1: IoSliceMut<'_> = IoSliceMut::new(unsafe {
        from_raw_parts_mut(
            &mut my_struct as *mut FileStat as *mut u8,
            size_of_val(&my_struct),
        )
    });

    let buffer2: IoSliceMut<'_> = IoSliceMut::new(unsafe {
        from_raw_parts_mut(&mut x as *mut i32 as *mut u8, size_of_val(&x))
    });

    let buffer3: IoSliceMut<'_> = IoSliceMut::new(&mut str);

    let mut buffers: [IoSliceMut; 3] = [buffer1, buffer2, buffer3];

    let mut f: File = File::open(av[1].as_str()).expect("can not open file");

    let tot_required: usize = size_of_val(&my_struct) + size_of_val(&x) + STR_SIZE;
    let num_read: usize = f
        .read_vectored(&mut buffers)
        .expect("can not perform scatter read");

    if num_read < tot_required {
        println!("Read fewer bytes than requested");
    }

    println!(
        "total bytes requested: {}; bytes read: {}",
        tot_required, num_read
    );
}
