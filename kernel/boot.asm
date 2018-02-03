MAGIC equ 0x53494829

global booting

section .sabt
	dd MAGIC
	
section .bss
	align 16
	stack_bottom:
	resb 16384 ; 16 KiB
	stack_top:
	
section .text
	booting:
		mov esp, stack_top
		
		call clear_screen
		
		mov dx, 0x3D4
		mov al, 0x0A
		out dx, al
		inc dx
		mov al, 0x20
		out dx, al
		
		; Print '!' in red color to know that everything is working!
		mov ebx, 0xB8000
		mov word [ebx], (0x0C << 8 | '!')
		
		hang:
			cli
			hlt
			jmp hang
		.end:
		
	clear_screen:
		mov ebx, 0xB8000
		mov ecx, 2000
		xor eax, eax
		.loop:
			mov word [ebx+eax], (0x07 << 8 | ' ')
			add eax, 2
			loop .loop
		.done:
		ret
	
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
	
