
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef $_HEAP_H
#define $_HEAP_H

typedef struct tag_heap_blockheader blockheader_t;
struct tag_heap_blockheader
{
	uint32_t block_address;
	bool used;
} __attribute__((packed));

void heap_init();
void *memalloc(size_t length);
void *memalloc_aligned(size_t length, uint32_t align);
void memfree(uint8_t *allocated_ptr);
float allocator_speedtest();

#endif