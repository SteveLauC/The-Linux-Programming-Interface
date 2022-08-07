mod alloc;
mod unistd;

use alloc::{Allocator, MemorySegment};

fn main() {
    let mut al: Allocator = Allocator::new();
    println!("{:?}", al);
    let new_mem: MemorySegment = al.malloc(4).unwrap();
    println!("{:?}", new_mem);

    unsafe { (new_mem.start.as_ptr() as *mut i32).write(4) };

    assert_eq!(unsafe { (new_mem.start.as_ptr() as *mut i32).read() }, 4);
    al.free(new_mem);
    println!("{:?}", al);
}
