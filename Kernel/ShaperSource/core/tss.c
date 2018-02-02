
#include <string.h>
#include <stdbool.h>
#include <sysprint.h>
#include <stdint.h>
#include <gdt.h>
#include <tss.h>

taskinfo_t tasks[256];

tss_t current_tss;
uint32_t current_tss_sel;

void _task_set(string task_name, int gdt_entry, bool hiddentask, uint16_t kernel_stack_sel, uint32_t kernel_stack_ptr)
{
	static tss_t* tss = 0xC000;
	tss += (gdt_entry - 5);
	_gdt_set_descriptor(gdt_entry, 0xC000, 0xC000 + sizeof(tss_t), 0x89, 0x00);
	
	printstr(" 0x");
	printstr(hex2str(tss->es, 4));
	printstr(" 0x");
	printstr(hex2str(tss->fs, 4));
	printstr(" 0x");
	printstr(hex2str(tss->gs, 4));
	printstr(" 0x");
	printstr(hex2str(tss->cs, 4));

	memclr(0x10F000, sizeof(tss_t));

	printstr(": 0x");
	printstr(hex2str(tss->es, 4));
	printstr(" 0x"); 
	printstr(hex2str(tss->fs, 4));
	printstr(" 0x");
	printstr(hex2str(tss->gs, 4));
	printstr(" 0x");
	printstr(hex2str(tss->cs, 4));

	tss->ss0 = kernel_stack_sel;
	tss->esp0 = kernel_stack_ptr;

	// First code is on the stack (might be changed)
	tss->cs = kernel_stack_sel;
	tss->eip = kernel_stack_ptr;

	tss->ds = tss->es = tss->fs = tss->gs = tss->ss = 0x13;

	taskinfo_t task_info;
	task_info.task_name = task_name;
	task_info.task_tss = tss;
	task_info.tss_selector = 0x08 * gdt_entry;
	task_info.task_id = (gdt_entry - 5);
	task_info.hidden_task = hiddentask;

	tasks[gdt_entry - 5] = task_info;
}

tss_t get_tss()
{
	return current_tss;
}