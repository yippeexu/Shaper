
#include <stdint.h>
#include <irq.h>

#ifndef $_ISR_H
#define $_ISR_H

void _isr_load();
void _isr_fault_handler(x86_regs *regs);

#endif