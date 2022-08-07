#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_MEM_SEG 100

typedef struct {
	void *start;
	uint64_t size;
} MemSegment;

typedef struct {
	uint64_t cur_program_break;
	MemSegment free_list[MAX_MEM_SEG];
	uint64_t len;
} Allocator;

void allocator_init(Allocator *const);
MemSegment allocator_malloc(Allocator *const, uint64_t);
void allocator_free(Allocator *const, const MemSegment *const);
void allocator_print(const Allocator *const);

void free_list_push(MemSegment, Allocator *const);
MemSegment free_list_remove(uint64_t, Allocator *const);
