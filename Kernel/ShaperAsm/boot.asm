; Declare constants for the multiinit0 header.
MAGIC   equ 0x1BADB002
FLAGS   equ (1 << 0) | (1 << 1)
CHECKSUM equ -(MAGIC + FLAGS)

extern init0
extern init1
extern _sys_realmode
global start
global start:function (.end - start)
	
section .multiinit0
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
section .bss
	align 16
	stack_bottom:
	resb 16384 ; 16 KiB
	stack_top:
	
section .text
;global ReloadSegmentRegs:function (ReloadSegmentRegs.reloadend - ReloadSegmentRegs)
	start:
		mov esp, stack_top
		;call _sys_realmode
		
		call init0
		;sti
		;add esp, 0C0000000h
		call init1
		
		hang:
			cli
			hlt
			jmp hang
		.end:
	
;ReloadSegmentRegs:
;	jmp 0x08:ReloadCS
;.ReloadCS:
;	mov ax, 0x10
;	mov ds, ax
;	mov es, ax
;	mov fs, ax
;	mov gs, ax
;	mov ss, ax
;	ret
;.reloadend
	
