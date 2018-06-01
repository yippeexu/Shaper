
#include <sysprint.h>
#include <stdbool.h>
#include <system.h>
#include <stdint.h>

#include <paging.h>
#include <heap.h>
#include <acpi.h>
#include <apic.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <irq.h>
#include <pic.h>

void init0()
{
	_sys_disableinterrupts();

	init_printer(CGA16_LIGHTGREY, CGA16_BLACK);
	serial_init(0);

	printinf("(CORE) Core Init");
	
	printinf("(CORE) Disabling Interrupts");

	printinf("(CORE) Loading IDT");
	_idt_preload();
	_idt_load();

	printinf("(CORE) Loading IRQ");
	_irq_load();

    _sys_enableinterrupts();

	printinf("(CORE) Initializing ACPI");
	acpi_init();

	printinf("(CORE) Switching to ACPI mode");
	acpi_switch();

	printinf("(CORE) Checking CPUID");
	cpuid_init();

	printinf("(CORE) Initializing Heap");
	heap_init();

	printinf("(CORE) Core Done");
	printchr('\n');
	for (;;) ;
}