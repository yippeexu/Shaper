
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <gdt.h>

extern gdt_t _gdt32[256];
size_t gdtentries;

void _gdt_init()
{
	_gdt_load();
}

gdt_t* _gdt_get_ptr()
{
	return _gdt32;
}

void _gdt_set_descriptor(int entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
	_gdt32[entry].base0 = base & 0xFFFF;
	_gdt32[entry].base1 = (base >> 16) & 0xFF;
	_gdt32[entry].base2 = (base >> 24) & 0xFF;

	_gdt32[entry].limit0 = limit & 0xFFFF;
	_gdt32[entry].granularity = ((limit >> 16) & 0x0F);

	_gdt32[entry].granularity |= flags & 0xF0;
	_gdt32[entry].access = access;
}