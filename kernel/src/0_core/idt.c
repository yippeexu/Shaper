
#include <stdint.h>
#include <string.h>
#include <idt.h>
#include <irq.h>
#include <isr.h>
#include <pic.h>

idt32_t _idt[256];
idt64_t _idt64[256];
idtr32_t _idtr;
idtr64_t _idtr64;

void _idt_preload()
{
	_idtr.base = _idt;
	_idtr.limit = sizeof(idt32_t) * 256 - 1;
	_idtr64.base = (uint64_t)_idt64;
	_idtr64.limit = sizeof(idt64_t) * 256 - 1;

	memclr((void *)_idtr.base, (size_t)_idtr.limit);
	memclr((void *)_idtr64.base, (size_t)_idtr64.limit);
	
	_isr_load();
	_pic_remap(0x20, 0x03, 0x28, 0x11);
}

void _idt_set_entry32(int entry, uint32_t offset, uint8_t typeattr, uint16_t selector)
{
	_idt[entry].offsetlow = (offset & 0xFFFF);
	_idt[entry].offsethigh = ((offset >> 16) & 0xFFFF);

	_idt[entry].resv = 0;
	_idt[entry].selector = selector;
	_idt[entry].typeattr = typeattr;
}

void _idt_set_entry64(int entry, uint64_t offset, uint8_t typeattr, uint16_t selector)
{
	_idt64[entry].offsetlow = (offset & 0xFFFF);
	_idt64[entry].offsetmid = ((offset >> 16) & 0xFFFF);
	_idt64[entry].offsethigh = (offset >> 32);

	_idt64[entry].selector = selector;
	_idt64[entry].typeattr = typeattr;
}

uint32_t _idt_get_base(void)
{
	return (uint32_t)&_idt;
	//return (uint32_t)&_idt64;
}