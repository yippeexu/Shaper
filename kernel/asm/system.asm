global _sys_enableinterrupts
global _sys_disableinterrupts
global _sys_bioscalls32
global _sys_enablepaging

extern page_dir

backup_ebx dd 0

_sys_disableinterrupts:
	cli
	ret
_sys_enableinterrupts:
	sti
	ret

%define rm_addr(x) 0x3000 + (x - _pm_16)

sys_bioscalls32:
_sys_bioscalls32: use32
	; Copy the program code to 0x3000 (below 1 MiB)
	mov edi, 0x3000
	mov esi, (_pm_16 - 0xC0100000 + 0x100000)
	mov ecx, 0xFFFF
	cld
	rep movsb

	mov eax, [esp+4]
	mov byte [rm_addr(_rm_intnum)], al

	mov edi, 0x67E6
	mov esi, [esp+8]
	mov ecx, 0xD
	cld
	rep movsw
	
	sidt [rm_addr(saved_idtr)]
	mov dword [rm_addr(saved_esp)], esp

	; 1. Disable the interrupts
	cli
 
	; 3. Use GDT with 16-bit tables
	; -- Using GDT with 32-bit and 16-bit entries

	; 4. Far-jump to 16-bit protected mode
	jmp 0x28:rm_addr(_pm_16)

_pm_16: use16
	; 5. Load data segment selectors with 16-bit indexes
	mov ax, 0x30
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; 6. Load real mode IDT (IVT)
	mov bp, 0x7000
	mov word [bp], 0x3FF
	mov word [bp+2], 0
	mov word [bp+4], 0 
	lidt [bp]
	
	; 7. Disable protected mode and paging
	mov eax, cr0
	and eax, 0x7FFFFFFE
	mov cr0, eax
	xor eax, eax
	mov cr3, eax

	; 8. Far-jump to real mode
	jmp dword 0:rm_addr(_rm_16)

_rm_16: use16
	; 9. Reload data segment registers with real mode values
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; 10. Remap PIC to Real Mode Values
	mov al, 0x11
	out 0x20, al
	out 0xA0, al
	mov al, 0x08
	out 0x21, al
	mov al, 0x70
	out 0xA1, al
	mov al, 0x04
	out 0x21, al
	mov al, 0x02
	out 0xA1, al
	mov al, 0x01
	out 0x21, al
	out 0xA1, al

	; 11. Set stack pointer to appropriate value
	mov esp, 0x6800-0x1A

	; 12. Enable interrupts
	sti

_rm_code: use16
	; 13. Continue on in real mode with all bios interrupts
	
	popa

	pop ds
	pop es
	pop fs
	pop gs
	pop ss

	db 0xCD
_rm_intnum: db 0x00

	push ss
	push gs
	push fs
	push es
	push ds
	pusha

	mov di, 5000h
	mov si, sp
	mov cx, 0xD
	cld
	rep movsw

	cli

	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp dword 0x08:rm_addr(_pm_32)

_pm_32: use32
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, [rm_addr(saved_esp)]

	mov eax, page_dir - 0xC0100000 + 0x100000
	mov cr3, eax
	
	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax

	mov eax, 5000h

	lidt [rm_addr(saved_idtr)]
	mov al, 0x11
	out 0x20, al
	out 0xA0, al
	mov al, 0x20
	out 0x21, al
	mov al, 0x28
	out 0xA1, al
	mov al, 0x04
	out 0x21, al
	mov al, 0x02
	out 0xA1, al
	mov al, 0x01
	out 0x21, al
	out 0xA1, al
	mov al, 0x03
	out 0x21, al
	mov al, 0x11
	out 0xA1, al

	sti
	ret

saved_esp:
	dd 0

saved_idtr:
	dw 0
	dd 0

_rm_end:

[bits 32]

_sys_enablepaging:
	mov eax, page_dir - 0xC0000000
	mov cr3, eax
	
	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax
	ret