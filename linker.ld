ENTRY(start)

SECTIONS
{
	. = 0xC0100000;
	
	.multiboot ALIGN(4K) : AT(ADDR(.multiboot) - 0xC0100000 + 0x100000)
	{
		*(.multiboot)
	}
 
	.text ALIGN(4K) : AT(ADDR(.text) - 0xC0100000 + 0x100000)
	{
		*(.text)
	}
 
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0100000 + 0x100000)
	{
		*(.rodata)
	}
 
	.data ALIGN(4K) : AT(ADDR(.data) - 0xC0100000 + 0x100000)
	{
		*(.data)
	}
 
	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0100000 + 0x100000)
	{
		*(.bss)
	}
}
