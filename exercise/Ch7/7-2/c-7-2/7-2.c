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
