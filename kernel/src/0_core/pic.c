
#include <stdbool.h>
#include <stdint.h>
#include <io.h>

void _pic_remap(int offset1, int mask1, int offset2, int mask2)
{
	uint8_t a1, a2;

	a1 = inb(0x21);
	a2 = inb(0xA1);

	outb(0x20, 0x11);
	io_sleep();
	outb(0xA0, 0x11);
	io_sleep();
	outb(0x21, offset1);
	io_sleep();
	outb(0xA1, offset2);
	io_sleep();
	outb(0x21, 4);
	io_sleep();
	outb(0xA1, 2);
	io_sleep();

	outb(0x21, 0x01);
	io_sleep();
	outb(0xA1, 0x01);
	io_sleep();

	outb(0x21, mask1);
	outb(0xA1, mask2);
}

void _pic_sendEOI(uint8_t irq)
{
	if (irq >= 8)
	{
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
}

bool _pic_in_service(uint8_t irq)
{
	uint16_t port = 0x20;
	if (irq >= 8) 
		port |= 0x80;
	
	irq %= 8;

	outb(port, 0x0B);
	return (inb(port) >> irq) & 1;
}