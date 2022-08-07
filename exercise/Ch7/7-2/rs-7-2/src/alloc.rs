#![deny(missing_docs)]

//! my memory allocator
use crate::unistd::sbrk;
use nix::Result;
use std::{os::raw::c_void, ptr::NonNull};

#[derive(Debug)]
/// A memory segment
pub struct MemorySegment {
    pub start: NonNull<c_void>,
    pub size: usize,
}

impl MemorySegment {
    /// Construct a new memory segment
    pub fn new(start: NonNull<c_void>, size: usize) -> Self {
        MemorySegment { start, size }
    }
}

/// A memory allocator
#[derive(Debug)]
pub struct Allocator {
    cur_program_break: usize,
    free_list: Vec<MemorySegment>,
}

impl Allocator {
    /// Construct a new memory allocator
    pub fn new() -> Self {
        Allocator {
            cur_program_break: sbrk(0).unwrap() as usize,
            free_list: Vec::new(),
        }
    }

    /// Merging memory fragments
    fn compact_free_list(&mut self) {
        self.free_list
            .sort_by(|a: &MemorySegment, b: &MemorySegment| a.start.partial_cmp(&b.start).unwrap());
    }

    /// Allocate memory
    ///
    /// The memory returned is **NOT** initialized
    pub fn malloc(&mut self, size: usize) -> Result<MemorySegment> {
        // iterate over the free list to find the first memory block whose
        // size can satify our requirements
        if let Some(idx) =
            (0..self.free_list.len()).find(|idx: &usize| self.free_list[*idx].size > size)
        {
            let mut chunk: MemorySegment = self.free_list.remove(idx);
            if chunk.size > size {
                self.free_list.push(MemorySegment::new(
                    NonNull::new((chunk.start.as_ptr() as usize + size) as *mut c_void).unwrap(),
                    chunk.size - size,
                ));
            }

            chunk.size = size;
            return Ok(chunk);
        }

        // increase the program break to allocate memory
        let old_program_break: usize = sbrk(
            isize::try_from(size).expect("overflow happens when converting from usize to isize"),
        )? as usize;

        self.cur_program_break = sbrk(0)? as usize;
        assert_eq!(size + old_program_break, self.cur_program_break);

        Ok(MemorySegment::new(
            NonNull::new(old_program_break as *mut c_void).unwrap(),
            size,
        ))
    }

    /// Deallocate memory specified by `mem`.
    pub fn free(&mut self, mem: MemorySegment) {
        self.free_list.push(mem);
    }
}
