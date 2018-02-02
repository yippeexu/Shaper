OUTPUT_FORMAT("elf32-i386");
ENTRY(boot_start);
SECTIONS
{
    . = 0x9C00;
    /* Code and data in start.o placed between 0x7c00 and 0x7e00 */
    .boot : SUBALIGN(0) {
        start.o(.text)
        start.o(.rodata*)
        start.o(.data)
    }

    /* Code and data from all other files placed after 0x7e00 */
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
