;MAGIC equ 0x53414254
MAGIC    equ 0x1BADB002
FLAGS    equ (1 << 0) | (1 << 1)
CHECKSUM equ -(MAGIC + FLAGS)

global start
global start:function (.end - start)
extern _gdtr
extern init
extern page_dir
extern paging_init

section .multiboot
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

;section .sabt
;	dd MAGIC
	
section .bss
	align 16
	stack_bottom:
	resb 16384 ; 16 KiB
	stack_top:
	
section .text
	start:
		lgdt [_gdtr - 0xC0100000 + 0x100000]
		jmp 0x08:(kernel - 0xC0100000 + 0x100000)

	kernel:
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		mov esp, (stack_top - 0xC0100000 + 0x100000)

		call paging_init

		mov eax, page_dir - 0xC0100000 + 0x100000
		mov cr3, eax
		
		mov eax, cr0
		or eax, 0x80000001
		mov cr0, eax

		jmp 0x08:higher_half

	higher_half:
		call init

		;jmp kernel32
		;jmp kernel64
		
		hang:
			cli
			hlt
			jmp hang
	.end: