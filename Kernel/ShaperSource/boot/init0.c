
#include <sysprint.h>
#include <system.h>
#include <stdbool.h>
#include <stdint.h>

#include <paging.h>
#include <heap.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <irq.h>
#include <tss.h>
#include <pic.h>
#include <pit.h>

//#define MAIN_PAGING_DIR_ADDRESS 0x00110000

void _keyhandler(x86_regs *context);

void init0()
{
	_sys_disableinterrupts();
	
	init_printer(CGA16_LIGHTGREY, CGA16_BLACK);

	printinf("Core Init");
	
	printinf("GDT Setup");
	_gdt_set_descriptor(0, 0, 0, 0, 0);
	_gdt_set_descriptor(1, 0x00000000, 0xFFFFFFFF, 0x9A, 0xCF);
	_gdt_set_descriptor(2, 0x00000000, 0xFFFFFFFF, 0x92, 0xCF);
	_gdt_set_descriptor(3, 0x00000000, 0xFFFFFFFF, 0xFA, 0xCF);
	_gdt_set_descriptor(4, 0x00000000, 0xFFFFFFFF, 0xF2, 0xCF);
	_gdt_set_descriptor(5, 0x00000000, 0x0000FFFF, 0x9A, 0x0F);
	_gdt_set_descriptor(6, 0x00000000, 0x0000FFFF, 0x92, 0x0F);
	_gdt_set_descriptor(7, 0x00000000, 0x0000FFFF, 0xFA, 0x0F);
	_gdt_set_descriptor(8, 0x00000000, 0x0000FFFF, 0xF2, 0x0F);
	_task_set("System", 10, true, 0x10, 0);
	_task_set("Virtual 8086 Task", 11, true, 0x10, 0);

	printinf("Loading GDT");
	_gdt_init();

	printinf("Loading System Task");
	_tss_load(0x50);

	//printinf("Enabling Interrupts");
	//_sys_enableinterrupts();

	printinf("Loading IDT");
	_idt_preload();
	_idt_load();

	printinf("Loading IRQ");
	_irq_load();
	//printinf("Add Keyboard press IRQ");
	//_irq_add_handler(1, _keyhandler);
	

	//printinf("Switching to Virtual 8086 Mode");
	//static void *vm8086entrance = (void*)0x9000;
	//vm8086entrance = vm8086_entrance;
	//_sys_v86mode(0, 0x9000, 0, 0x9000);
 

	printinf("Paging Setup");
	//identify_paging();
	paging_init();
	printinf("Enabling Paging");
	_sys_enablepaging(0x108000);

	printinf("Test Higherhalf Framebuffer");
	uint16_t* higherhalf_vgabuffer = (uint16_t*)0xB8000;
	higherhalf_vgabuffer[1920] = (0x0C << 8 | 'B');
	higherhalf_vgabuffer[1921] = (0x0C << 8 | 'o');
	higherhalf_vgabuffer[1922] = (0x0C << 8 | 'm');
	higherhalf_vgabuffer[1923] = (0x0C << 8 | '!');

	printstr("Allocted address with size 32 bytes: ");
	heap_init();
	printstr(hex2str(memalloc(0x20), 8));

	//map_page(0x000B8000, 0x0000C0DE, 3);
	//printstr(hex2str(get_page_physaddr(0xC0DE), 8));
	
	//map_page(0xB8000, 0xAC41);
	map_page(0x00130000, 0xC0001000, 3);
	char* pagetest = 0xC0001000;
	pagetest[0] = 'W';
	pagetest[1] = 'o';
	pagetest[2] = 'w';
	pagetest[3] = ',';
	pagetest[4] = ' ';
	pagetest[5] = 'Y';
	pagetest[6] = 'o';
	pagetest[7] = 'u';
	pagetest[8] = ' ';
	pagetest[9] = 'd';
	pagetest[10] = 'i';
	pagetest[11] = 'd';
	pagetest[12] = ' ';
	pagetest[13] = 't';
	pagetest[14] = 'h';
	pagetest[15] = 'i';
	pagetest[16] = 's';
	pagetest[17] = '?';
	uint8_t* check = 0x00130000;
	for (int i = 0; i < 32; i++)
	{
		printstr(" :");
		printchr(check[i]);
		printchr(':');
		printchr(pagetest[i]);
		printstr(":");
	}

	// pit_init();

	printstrf("%cg DONE!%cD\n");
}

void init1()
{
	printstrf("\n\n\n   %bGGood!%b0");
	for (;;)
		asm("hlt");
}

void _keyhandler(x86_regs *context)
{
	printstrf("%cRW%cBo%cGo%cYo%cWw%cD.");
}