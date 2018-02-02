
#include <stdint.h>
#include <system.h>
#include <paging.h>

void paging_init()
{
	//pagedir_t* page_dir = (pagedir_t*)0x200000;
	//pagetable_t* page_table = (pagetable_t*)0x201000;
	//
	//for (int i = 0; i < 1024; i++)
	//{
	//	page_table[i].page_address = (4096 * i);
	//	page_table[i].userpage = 0;
	//	page_table[i].present = 1;
	//	page_table[i].rw = 1;
	//}
	//
	//page_dir[0].table_address = 0x201000;
	//page_dir[0].userpage = 0;
	//page_dir[0].present = 1;
	//page_dir[0].rw = 1;
	//
	//for (int i = 1; i < 1024; i++)
	//{
	//	page_dir[i].userpage = 0;
	//	page_dir[i].present = 0;
	//	page_dir[i].rw = 1;
	//}

	//uint32_t* page_dir = (uint32_t*)0x200000;
	//uint32_t* page_table = (uint32_t*)0x201000;
	//
	//for (int i = 0; i < 1024; i++)
	//	page_table[i] = (0x1000 * i) | 3;
	//
	//page_dir[0] = (uint32_t)0x201000 | 3;
	//
	//for (int i = 1; i < 1024; i++)
	// 	page_dir[i] = 0 | 2;

	uint32_t page_dir[1024] __attribute__((aligned(4096)));

	for (int i = 0; i < 1024; i++)
		page_dir[i] = 0x00000002;
	
	uint32_t page_table[1024] __attribute__((aligned(4096)));
	for (int i = 0; i < 1024; i++)
		page_table[i] = (i * 0x1000) | 3;
	
	// Set up 32 Page Table
	for (int i = 0; i < 1024; i++)
		page_dir[i] = (uint32_t)(page_table + (0x1000 * i)) | 3;

	//for (int i = 0; i < 1024; i++)
	//{
	//	map_page(0x1000 * i, 0xC0000000 + (0x1000 * i), 3);
	//}
}

void map_page(void* physical_address, void* virtual_address, uint16_t flags)
{
	uint32_t pdindex = (uint32_t)virtual_address >> 22;
	uint32_t ptindex = (uint32_t)virtual_address >> 12 & 0x03FF;

	uint32_t* pd = (uint32_t*)0xFFFFF000;
	uint32_t* pt = ((uint32_t*)0xFFC00000) + (0x400 * pdindex);

	pt[ptindex] = ((uint32_t)physical_address) | (flags & 0xFFF) | 0x01;
}

void* get_page_physaddr(void* virtual_address)
{
	uint32_t pdindex = (uint32_t)virtual_address >> 22;
	uint32_t ptindex = (uint32_t)virtual_address >> 12 & 0x03FF;

	uint32_t* pd = (uint32_t*)0xFFFFF000;
	uint32_t* pt = ((uint32_t*)0xFFC00000) + (0x400 * pdindex);

	return (void *)((pt[ptindex] & ~0xFFF) + ((uint32_t)virtual_address & 0xFFF));
}