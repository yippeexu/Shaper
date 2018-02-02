
#include <stdint.h>
#include <sysprint.h>
#include <irq.h>
#include <io.h>

int timerticks = 0;

void pit_handler(x86_regs *r);
void timer_phase(int hz);

void pit_handler(x86_regs *r)
{
	timerticks++;

	if ((timerticks % 18) == 0)
		printstr("One second has been passed");
}

void pit_init()
{
	_irq_add_handler(0, (unsigned)pit_handler);
}

void timer_phase(int hz)
{
	int divisor = 1193180 / hz;
	outb(0x43, 0x36);
	outb(0x40, divisor & 0xFF);
	outb(0x40, divisor >> 8);
}

void sleep(int ticks)
{
	uint32_t eticks;

	eticks = timerticks + ticks;
	while (timerticks > eticks);
}