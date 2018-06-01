
#include <stdint.h>

#ifndef $_IDT_H
#define $_IDT_H

typedef struct tag_idt32 idt32_t;
struct tag_idt32
{
	uint16_t offsetlow;
	uint16_t selector;
	uint8_t resv;
	uint8_t typeattr;
	uint16_t offsethigh;
} __attribute__((packed));

typedef struct tag_idtr idtr32_t;
struct tag_idtr
{
	uint16_t limit;
	idt32_t *base;
} __attribute__((packed));

typedef struct tag_idt64 idt64_t;
struct tag_idt64
{
	uint16_t offsetlow;
	uint16_t selector;
	uint8_t ist;
	uint8_t typeattr;
	uint16_t offsetmid;
	uint32_t offsethigh;
	uint32_t resv;
} __attribute__((packed));

typedef struct tag_idtr64 idtr64_t;
struct tag_idtr64
{
	uint16_t limit;
	uint64_t base;
} __attribute__((packed));

void _idt_preload();
void _idt_set_entry32(int entry, uint32_t offset, uint8_t typeattr, uint16_t selector);
void _idt_set_entry64(int entry, uint64_t offset, uint8_t typeattr, uint16_t selector);
uint32_t _idt_get_base();
extern void _idt_load();

#endif