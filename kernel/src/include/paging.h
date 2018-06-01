
#include <stdint.h>

#ifndef $_PAGING_H
#define $_PAGING_H

typedef struct tag_pagedir pagedir_t;
struct tag_pagedir
{
	uint8_t present : 1; // If set, the page is in the physical memory
	uint8_t rw : 1; // If set, the page is rw (Read/Write) otherwise the page is ro (Read only)
	uint8_t userpage : 1; // If set, User can access to all pages in this directory
	uint8_t writethrough : 1; // If set, write-through caching is enabled
	uint8_t nocaching : 1; // If set, the page will not be cached
	uint8_t accessed : 1; // If a page has been read or written, then this bit will be set
	uint8_t reserved : 1;
	uint8_t pagesize : 1; // If set, pages should be 4 MiB in size
	uint8_t ignored : 1; 
	uint8_t unused : 3;
	uint32_t table_address : 20;
} __attribute__((packed));

typedef struct tag_pagetable pagetable_t;
struct tag_pagetable
{
	uint8_t present : 1; // If set, the page is in the physical memory
	uint8_t rw : 1; // If set, the page is rw (Read/Write) otherwise the page is ro (Read only)
	uint8_t userpage : 1; // If set, User can access to all pages in this table
	uint8_t writethrough : 1; // If set, write-through caching is enabled
	uint8_t nocaching : 1; // If set, the page will not be cached
	uint8_t accessed : 1; // If a page has been read or written, then this bit will be set
	uint8_t dirty : 1; // If a page has been written, then this bit will be set
	uint8_t reserved : 1;
	uint8_t global : 1;
	uint8_t unused : 3;
	uint32_t page_address : 20;
} __attribute__((packed));

void paging_init();

#endif