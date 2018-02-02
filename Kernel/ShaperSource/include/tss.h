
#include <stdint.h>
#include <stdbool.h>

#ifndef $_TSS_H
#define $_TSS_H

typedef struct tag_tss tss_t;
struct tag_tss
{
	// link
	uint16_t link;
	uint16_t linkreserved;

	// esp0
	uint32_t esp0;

	// ss0
	uint16_t ss0;
	uint16_t ss0reserved;

	// esp1
	uint32_t esp1;

	// ss1
	uint16_t ss1;
	uint16_t ss1reserved;

	// esp2
	uint32_t esp2;

	// ss2
	uint16_t ss2;
	uint16_t ss2reserved;

	// other registers
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;

	// segments
	uint16_t es;
	uint16_t esreserved;

	uint16_t cs;
	uint16_t csreserved;

	uint16_t ss;
	uint16_t ssreserved;

	uint16_t ds;
	uint16_t dsreserved;

	uint16_t fs;
	uint16_t fsreserved;

	uint16_t gs;
	uint16_t gsreserved;

	uint16_t ldtr;
	uint16_t ldtrreserved;

	uint16_t iopboffsetreserved;
	uint16_t iopboffset;

} __attribute__((packed));

typedef struct tag_taskinfo
{
	tss_t* task_tss;
	string task_name;
	uint16_t tss_selector;
	size_t task_id;
	bool hidden_task;

} taskinfo_t;

tss_t get_tss();
void _task_set(string task_name, int gdt_entry, bool hiddentask, uint16_t kernel_stack_sel, uint32_t kernel_stack_ptr);

extern void _tss_load(uint32_t tss_sel);

#endif