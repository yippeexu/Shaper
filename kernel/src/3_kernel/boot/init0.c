
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
	init_printer(CGA16_LIGHTGREY, CGA16_BLACK);
	serial_init(0);

	printinf("core: disabling interrupts");
	_sys_disableinterrupts();

	printinf("core: loading idt");
	_idt_preload();
	_idt_load();

	printinf("core: remapping pic and seting up irq");
	_irq_load();

	printinf("core: enabling interrupts");
    _sys_enableinterrupts();

	printinf("core: intializing acpi");
	acpi_init();

	printinf("core: switching to acpi mode");
	acpi_switch();

	cpuid_init();

	printinf("core: initializing memory allocator (heap)");
	heap_init();
}