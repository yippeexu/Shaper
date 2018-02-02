
#include <io.h>
#include <irq.h>
#include <idt.h>
#include <sysprint.h>
#include <pic.h>

#define TIMER_IRQ 0
#define KEY

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

// IRQ Handlers array
void *_irq_handlers[] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void _irq_add_handler(int irq, void(*handler)(x86_regs *context))
{
	_irq_handlers[irq] = handler;
}

void _irq_remove_handler(int irq, void(*handler)(x86_regs *context))
{
	_irq_handlers[irq] = 0;
}

void _irq_remap(void)
{
	outb(0x0020, 0x11);
	io_sleep();
	outb(0x00A0, 0x11);
	io_sleep();
	outb(0x0021, 0x20);
	io_sleep();
	outb(0x00A1, 0x28);
	io_sleep();
	outb(0x0021, 0x04);
	io_sleep();
	outb(0x00A1, 0x02);
	io_sleep();
	outb(0x0021, 0x01);
	io_sleep();
	outb(0x00A1, 0x01);
	io_sleep();
	outb(0x0021, 0);
	io_sleep();
	outb(0x00A1, 0);
	io_sleep();
}

void _irq_load()
{
	_irq_remap();

	_idt_set_entry32(32, (unsigned)_irq0, 0x8E, 0x08);
	_idt_set_entry32(33, (unsigned)_irq1, 0x8E, 0x08);
	_idt_set_entry32(34, (unsigned)_irq2, 0x8E, 0x08);
	_idt_set_entry32(35, (unsigned)_irq3, 0x8E, 0x08);
	_idt_set_entry32(36, (unsigned)_irq4, 0x8E, 0x08);
	_idt_set_entry32(37, (unsigned)_irq5, 0x8E, 0x08);
	_idt_set_entry32(38, (unsigned)_irq6, 0x8E, 0x08);
	_idt_set_entry32(39, (unsigned)_irq7, 0x8E, 0x08);
	_idt_set_entry32(40, (unsigned)_irq8, 0x8E, 0x08);
	_idt_set_entry32(41, (unsigned)_irq9, 0x8E, 0x08);
	_idt_set_entry32(42, (unsigned)_irq10, 0x8E, 0x08);
	_idt_set_entry32(43, (unsigned)_irq11, 0x8E, 0x08);
	_idt_set_entry32(44, (unsigned)_irq12, 0x8E, 0x08);
	_idt_set_entry32(45, (unsigned)_irq13, 0x8E, 0x08);
	_idt_set_entry32(46, (unsigned)_irq14, 0x8E, 0x08);
	_idt_set_entry32(47, (unsigned)_irq15, 0x8E, 0x08);
}				

void _irq_ignore(int irq)
{
	uint16_t port = 0x20;
	uint8_t value;

	if (irq > 8)
	{
		port += 0x80;
		irq -= 8;
	}

	value = inb(port) | (1 << irq);
	outb(port, value);
}

void irq_handler(x86_regs *context)	   
{			
	printstrf("%cRecieved interrupt ");
	printstr(hex2str(context->interrupt_no, 2));
	printstr(" : IRQ");
	printnum(context->interrupt_no - 32);

	void(*handler)(x86_regs *context);

	handler = _irq_handlers[context->interrupt_no - 0x20];
	if (handler && (context->interrupt_no- 32 != TIMER_IRQ))
	{
		handler(context);
	}

	_pic_sendEOI(context->interrupt_no - 32);

	if (handler && context->interrupt_no - 32 == 0)
		handler(context);
}