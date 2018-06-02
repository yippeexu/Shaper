
#include <sysprint.h>
#include <stdbool.h>
#include <string.h>

#include <idt.h>
#include <isr.h>

extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

void _isr_load()
{
	_idt_set_entry32(0, (unsigned)_isr0, 0x8E, 0x08);
	_idt_set_entry32(1, (unsigned)_isr1, 0x8E, 0x08);
	_idt_set_entry32(2, (unsigned)_isr2, 0x8E, 0x08);
	_idt_set_entry32(3, (unsigned)_isr3, 0x8E, 0x08);
	_idt_set_entry32(4, (unsigned)_isr4, 0x8E, 0x08);
	_idt_set_entry32(5, (unsigned)_isr5, 0x8E, 0x08);
	_idt_set_entry32(6, (unsigned)_isr6, 0x8E, 0x08);
	_idt_set_entry32(7, (unsigned)_isr7, 0x8E, 0x08);
	_idt_set_entry32(8, (unsigned)_isr8, 0x8E, 0x08);
	_idt_set_entry32(9, (unsigned)_isr9, 0x8E, 0x08);
	_idt_set_entry32(10, (unsigned)_isr10, 0x8E, 0x08);
	_idt_set_entry32(11, (unsigned)_isr11, 0x8E, 0x08);
	_idt_set_entry32(12, (unsigned)_isr12, 0x8E, 0x08);
	_idt_set_entry32(13, (unsigned)_isr13, 0x8E, 0x08);
	_idt_set_entry32(14, (unsigned)_isr14, 0x8E, 0x08);
	_idt_set_entry32(15, (unsigned)_isr15, 0x8E, 0x08);
	_idt_set_entry32(16, (unsigned)_isr16, 0x8E, 0x08);
	_idt_set_entry32(17, (unsigned)_isr17, 0x8E, 0x08);
	_idt_set_entry32(18, (unsigned)_isr18, 0x8E, 0x08);
	_idt_set_entry32(19, (unsigned)_isr19, 0x8E, 0x08);
	_idt_set_entry32(20, (unsigned)_isr20, 0x8E, 0x08);
	_idt_set_entry32(21, (unsigned)_isr21, 0x8E, 0x08);
	_idt_set_entry32(22, (unsigned)_isr22, 0x8E, 0x08);
	_idt_set_entry32(23, (unsigned)_isr23, 0x8E, 0x08);
	_idt_set_entry32(24, (unsigned)_isr24, 0x8E, 0x08);
	_idt_set_entry32(25, (unsigned)_isr25, 0x8E, 0x08);
	_idt_set_entry32(26, (unsigned)_isr26, 0x8E, 0x08);
	_idt_set_entry32(27, (unsigned)_isr27, 0x8E, 0x08);
	_idt_set_entry32(28, (unsigned)_isr28, 0x8E, 0x08);
	_idt_set_entry32(29, (unsigned)_isr29, 0x8E, 0x08);
	_idt_set_entry32(30, (unsigned)_isr30, 0x8E, 0x08);
	_idt_set_entry32(31, (unsigned)_isr31, 0x8E, 0x08);
}										   
char *_exception_messages[] =
{
	"Division by Zero              ",
	"Debug                         ",
	"Non Maskable Interrupt        ",
	"Breakpoint                    ",
	"Overflow                      ",
	"Bound Range Exceeded          ",
	"Invaild Opcode                ",
	"Device Not Available          ",
	"Double Fault                  ",
	"Coproccessor Segment Overrun  ",
	"Invaild TSS                   ",
	"Segment Not Present           ",
	"General Protection Fault      ",
	"Page Fault                    ",
	"NaE                           ",
	"x87 Floating-Point Exception  ",
	"Alignment Check               ",
	"Machine Check                 ",
	"SIMD Floating-Point Exception ",
	"Virtualization Exception      ",
	"NaE                           ",
	"NaE                           ",
	"NaE                           ",
	"NaE                           ",
	"NaE                           ",
	"NaE                           ",
	"NaE                           ",
	"NaE                           ",
	"NaE                           ",
	"NaE                           ",
	"Security Exception            ",
	"NaE                           "
};								   

void _isr_fault_handler(x86_regs *context) 
{
	if (context->interrupt_no < 0x20)	   
	{
		set_cursor_visible(false);
		if (get_x_pos()) printchr('\n');
		printstrf("%cR%n");
		printstr("** x86 CPU Exception occurred **\n");
		printstr(_exception_messages[context->interrupt_no]);
		printstr(" ");
		sprintstr(0, "CPU Exception: ");
		sprintstr(0, _exception_messages[context->interrupt_no]);
		sprintstr(0, "\nx86 Register Table:\n");
		sprintstr(0, "EAX: ");
		sprinthex(0, context->eax, 0);
		sprintstr(0, ", EBX: ");
		sprinthex(0, context->ebx, 0);
		sprintstr(0, ", CS: ");
		sprinthex(0, context->cs, 0);
		sprintstr(0, "\nECX: ");
		sprinthex(0, context->ecx, 0);
		sprintstr(0, ", EDX: ");
		sprinthex(0, context->edx, 0);
		sprintstr(0, ", DS: ");
		sprinthex(0, context->ds, 0);
		sprintstr(0, "\nESP: ");
		sprinthex(0, context->esp, 0);
		sprintstr(0, ", EBP: ");
		sprinthex(0, context->ebp, 0);
		sprintstr(0, ", ES: ");
		sprinthex(0, context->es, 0);
		sprintstr(0, "\nEDI: ");
		sprinthex(0, context->edi, 0);
		sprintstr(0, ", ESI: ");
		sprinthex(0, context->esi, 0);
		sprintstr(0, ", FS: ");
		sprinthex(0, context->fs, 0);
		sprintstr(0, "\nEIP: ");
		sprinthex(0, context->eip, 0);
		sprintstr(0, ", GS: ");
		sprinthex(0, context->gs, 0);
		sprintstr(0, ", SS: ");
		sprinthex(0, context->ss, 0);
		sprintstr(0, "\nEFLAGS: ");
		sprinthex(0, context->eflags, 0);
		sprintstr(0, "\nError Code: ");
		sprinthex(0, context->error_code, 0);
		sprintchr(0, '\n');
		while (1);
	}
}
