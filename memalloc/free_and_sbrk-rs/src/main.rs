use libc::sbrk;
use std::{
    alloc::{alloc, dealloc, Layout},
    env::args,
    process::exit,
    ptr::null_mut,
};

const MAX_ALLOCS: usize = 10000;

fn main() {
    let av: Vec<usize> = args()
        .skip(1)
        .map(|s: String| s.parse::<usize>().unwrap())
        .collect();

    if av.len() < 2 {
        eprintln!("free_and_sbrk num-allocs block-size [step [min [max]]]");
        exit(1);
    }

    let mut ptr: [*mut u8; MAX_ALLOCS] = [0; MAX_ALLOCS].map(|_| null_mut());
    let num_allocs: usize = av[0];
    let block_size: usize = av[1];

    let step: usize = if av.get(2).is_some() { av[2] } else { 1 };
    let min: usize = if av.get(3).is_some() { av[3] } else { 1 };
    let max: usize = if av.get(4).is_some() {
        av[4]
    } else {
        num_allocs
    };

    if max > num_allocs {
        eprintln!("max > num-allocs");
        exit(1);
    }

    println!("Initial program break: {:20p}", unsafe { sbrk(0) });
    println!("Allocating {} * {} bytes", num_allocs, block_size);

    let layout: Layout = Layout::from_size_align(block_size, 4).unwrap();

    for i in 0..num_allocs {
        ptr[i] = unsafe { alloc(layout) };
        if ptr[i].is_null() {
            eprintln!("malloc");
            exit(1);
        }
    }

    println!("Program break is now: {:20p}", unsafe { sbrk(0) });

    println!(
        "Freeing blocks from {} to {} in steps of {}",
        min, max, step
    );

    for i in ((min - 1)..max).step_by(step) {
        if !ptr[i].is_null() {
            unsafe { dealloc(ptr[i], layout) };
        }
    }

    println!("After free(), program break is: {:20p}", unsafe { sbrk(0) });
}
