OUTPUT_FORMAT("elf32-i386");
ENTRY(booting);
SECTIONS
{
    . = 0x10000; /* Proteted Mode Kernel's recommended address */
    /* Code and data in start.o placed between 0x10000 and 0x10200 */
    .boot : SUBALIGN(0) {
        kernel\obj\boot.o(.text)
        kernel\obj\boot.o(.rodata*)
        kernel\obj\boot.o(.data)
		kernel\obj\boot.o(.sabt)
    }

    /* Code and data from all other files placed after 0x10200 */
    .kernel.text : SUBALIGN(4) {
        *(.text)
    }
    .kernel.rodata : SUBALIGN(4) {
        *(.rodata*)
    }
    .kernel.data : SUBALIGN(4) {
        *(.data)
    }

    .kernel.bss : SUBALIGN(4) {
        __bss_start = .;
        *(COMMON);
        *(.bss)
        . = ALIGN(4);
        __bss_end = .;
    }
    __bss_sizel = SIZEOF(.kernel.bss)>>2;
    __bss_sizeb = SIZEOF(.kernel.bss);

    /* Remove unnecessary sections */
    /DISCARD/ : {
        *(.eh_frame);
        *(.comment);
        *(.note*);
    }


}
