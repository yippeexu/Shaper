
#include "stdint.h"
#include "io.h"

uint8_t inb(uint16_t port)
{
	uint16_t ret;
	__asm volatile ("inb %%dx, %%al"
		: "=a" (ret)
		: "Nd" (port));
	return ret;
}

uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm volatile ("inw %%dx, %%ax"
		: "=a" (ret)
		: "Nd" (port));
	return ret;
}

uint32_t inl(uint16_t port)
{
	uint16_t ret;
	__asm volatile ("inl %%dx, %%eax"
		: "=a" (ret)
		: "Nd" (port));
	return ret;
}

void outb(uint16_t port, uint8_t value)
{
	__asm volatile ("outb %%al, %%dx"
		: "=a" (value)
		: "Nd" (port));
}

void outw(uint16_t port, uint16_t value)
{
	__asm volatile ("outw %%ax, %%dx"
		: "=a" (value)
		: "Nd" (port));
}

void outl(uint16_t port, uint32_t value)
{
	__asm volatile ("outl %%eax, %%dx"
		: "=a" (value)
		: "Nd" (port));
}

void io_sleep()
{
	inb(0x00);
}