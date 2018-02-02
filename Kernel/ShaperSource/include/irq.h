
#ifndef $_IRQ_H
#define $_IRQ_H

typedef struct tag_x86regs
{
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t interrupt_no, error_code;
	uint32_t eip, cs, eflags, useresp, ss;
} x86_regs;

void _irq_add_handler(int irq, void(*handler)(x86_regs *context));
void _irq_remove_handler(int irq, void(*handler)(x86_regs *context));
void _irq_remap(void);
void _irq_load();
void _irq_ignore(int irq);
void irq_handler(x86_regs *context);
#endif