
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <heap.h>

#define PAGE_START 0x1000000

blockheader_t *block_header; // Allocated addresses are stored here to identify the block
uint32_t free_ptr;
size_t num_of_allocated_blocks = 0;
bool freeblock_found = false;
int freeblock_idx = 0;

void heap_init()
{
	free_ptr = PAGE_START;
	block_header = free_ptr;
	memclr(free_ptr, sizeof(blockheader_t) * 65536);
	free_ptr += sizeof(blockheader_t) * 65536;

}

void *memalloc(size_t length)
{
	if (!freeblock_found)
	{
		block_header[num_of_allocated_blocks].used = true;
		block_header[num_of_allocated_blocks].block_address = free_ptr;
		memclr((void *)block_header[num_of_allocated_blocks].block_address, length);
		free_ptr += length;
		return (void *)block_header[num_of_allocated_blocks++].block_address;
	}
	else
	{
		int i = 0;
		for (i = freeblock_idx; i < num_of_allocated_blocks; i++)
		{
			if (!block_header[i].used)
			{
				break;
			}
		}

		if (i == num_of_allocated_blocks)
		{
			uint32_t return_addr = block_header[freeblock_idx].block_address; 
			block_header[freeblock_idx].used = true;
			freeblock_found = false;
			freeblock_idx = 0;
			return (void *)return_addr;
		}
		else
		{
			uint32_t return_addr = block_header[freeblock_idx].block_address; 
			block_header[freeblock_idx].used = true;
			freeblock_idx = i;
			return (void *)return_addr;
		}
	}
}

void *memalloc_aligned(size_t length, uint32_t align)
{
	uint32_t freeptr_addr = free_ptr;
	while (true) 
	{
		if (!(freeptr_addr % align)) break;
		free_ptr = ++freeptr_addr;
	}
	return memalloc(length);
}

void memfree(uint8_t *allocated_ptr)
{
	for (int i = 0; i < num_of_allocated_blocks; i++)
	{
		if (!block_header[i].used) continue;

		if (block_header[i].block_address == (uint32_t)allocated_ptr)
		{
			memcpy((void *)&block_header[i], (void *)&block_header[i+1], (num_of_allocated_blocks-(i+1))*sizeof(blockheader_t));
            memclr((void *)&block_header[--num_of_allocated_blocks], sizeof(blockheader_t));
			if (i == num_of_allocated_blocks-1)
			{
				free_ptr = block_header[i-1].block_address;
			}
			else
			{
				block_header[i].used = false;
				freeblock_found = true;
				freeblock_idx = i;
			}
			break;
		}
	}
	allocated_ptr = 0;
}

float allocator_speedtest()
{
	// stopwatch_start();
	// memalloc(0x100000);
	// stopwatch_stop();
	// return stopwatch_get_ms() / 1000;
}