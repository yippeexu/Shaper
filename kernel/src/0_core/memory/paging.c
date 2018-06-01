
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <system.h>

#include <paging.h>
#include <heap.h>

pagedir_t page_dir[1024] __attribute__((aligned(4096)));// = //memalloc_aligned(4096, 4096);
pagetable_t page_table[1024*1024] __attribute__((aligned(4096)));//= //memalloc_aligned(4194304, 4096);

void paging_init()
{
    pagedir_t *physical_page_dir = (uint32_t)page_dir - 0xC0100000 + 0x100000;
    pagetable_t *physical_page_table = (uint32_t)page_table - 0xC0100000 + 0x100000;
    for (int pd_i = 0; pd_i < 1024; pd_i++)
    {
        physical_page_dir[pd_i].table_address = ((uint32_t)physical_page_table + pd_i * 4096) >> 12;
        physical_page_dir[pd_i].present = true;
        physical_page_dir[pd_i].rw = true;
        for (int pt_i = pd_i*1024; pt_i < pd_i*1024+1024; pt_i++)
        {
            physical_page_table[pt_i].page_address = (pt_i * 4096) >> 12;
            physical_page_table[pt_i].present = true;
            physical_page_table[pt_i].rw = true;
        }
    }

    for (int pd_i = 0; pd_i < 4; pd_i++)
    {
        physical_page_dir[768+pd_i].table_address = ((uint32_t)physical_page_table + pd_i * 4096) >> 12;
    }
}