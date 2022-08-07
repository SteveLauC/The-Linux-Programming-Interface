#include "malloc.h"

void allocator_init(Allocator *const p)
{
	p->cur_program_break = (uint64_t)sbrk(0);
	p->len = 0;
}

void allocator_print(const Allocator *const p)
{
	printf("program break: %ld", p->cur_program_break);
	printf("  free list: \n");
	for (uint64_t i = 0; i < p->len; i += 1) {
		printf("\t{start = %p, size = %ld}\n", p->free_list[i].start,
		       p->free_list[i].size);
	}
	printf("\n");
}
MemSegment free_list_remove(uint64_t idx, Allocator *const p)
{
	MemSegment m = p->free_list[idx];

	for (uint64_t i = idx; i < p->len; i += 1) {
		p->free_list[i] = p->free_list[i + 1];
	}
	p->len -= 1;

	return m;
}

void free_list_push(MemSegment item, Allocator *const p)
{
	p->free_list[p->len] = item;
	p->len += 1;
}

MemSegment allocator_malloc(Allocator *const p, uint64_t size)
{
	MemSegment res;
	int found = 0;
	for (uint64_t i = 0; i < p->len; i += 1) {
		if (p->free_list[i].size > size) {
			res = free_list_remove(i, p);
			found = 1;
			break;
		}
	}

	if (found) {
		if (res.size > size) {
			MemSegment redundant = {
				.start = (void *)((char *)res.start + size),
				.size = res.size - size,
			};
			free_list_push(redundant, p);

			res.size = size;
		}

		return res;
	}

	res.start = sbrk(size);
	p->cur_program_break = (uint64_t)sbrk(0);
	res.size = size;
	return res;
}

void allocator_free(Allocator *const p, const MemSegment *const p_to_mem)
{
	free_list_push(*p_to_mem, p);
}
