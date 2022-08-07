/*
 * QUESTION: (Advanced)Implement `malloc()` and `free()`.
 *
 * ANSWER:
 * 1. malloc: Maintain a free list, first we iterate over the free list to find the
 * first free memory segment whose size is bigger than or equal to our request 
 * size. If found, put the redundant part (if exists) to free list and return it.
 * If not found, call `sbrk(size)` to increase the `program break`, then return it.
 *
 * 2. free: simply add the memory segment to our free list.
*/

#include "malloc.h"

int main(void)
{
	Allocator a;
	allocator_init(&a);
	allocator_print(&a);
	MemSegment new_mem = allocator_malloc(&a, 4);
	allocator_print(&a);
	allocator_free(&a, &new_mem);
	allocator_print(&a);
}
