#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef struct tag_gdt gdt_t;
struct tag_gdt
{
	uint16_t limit0;
	uint16_t base0;
	uint8_t base1;
	uint8_t access;
	uint8_t granularity;
	uint8_t base2;
} __attribute__((packed));

typedef struct tag_gdtr gdtr_t;
struct tag_gdtr
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

gdt_t* _gdt_get_ptr();
void _gdt_init();
void _gdt_set_descriptor(int entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

extern void _gdt_load();