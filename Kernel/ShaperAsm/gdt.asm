;global _gdt_load
;;extern _gdtr
;_gdt32:
;	; Null entry
;    dw 0x00000
;    dw 0
;	db 0
;	db 0x00
;	db 0x00
;	db 0
;	
;	; Kernel code segment
;    dw 0x0FFFF
;    dw 0
;    db 0
;    db 0x9A
;    db 0xCF
;    db 0
;
;	; Kernel data segment
;    dw 0x0FFFF
;    dw 0
;    db 0
;    db 0x92
;    db 0xCF
;    db 0
;	
;	; User code segment
;    dw 0x0FFFF
;    dw 0
;    db 0
;    db 0xFA
;    db 0xCF
;    db 0
;	
;	; User code segment
;    dw 0x0FFFF
;    dw 0
;    db 0
;    db 0xF2
;    db 0xCF
;    db 0
;	
;	; Task state segment
;    dw 0xC068
;    dw 0x00
;    db 0xC0
;    db 0x89
;    db 0x00
;    db 0x00
;	
;	; Virtual 8086 Task state segment
;	dw 0xC0D0
;	dw 0x00
;	db 0xC0
;	db 0x89
;	db 0x00
;	db 0x68
;_gdt32_end:
;_gdtr:
;	dw _gdt32_end - _gdt32 - 1
;	dd _gdt32
;_gdt_load:
;	mov   AX, [ESP + 4]
;	mov   BX, [ESP + 8]
;	mov   CL, [ESP + 12]
;	mov   DL, [ESP + 16]
;	mov [_gdt32+40], AX
;	mov [_gdt32+42], BX
;	mov [_gdt32+44], CL
;	mov [_gdt32+47], DL
;	lgdt [_gdtr]
;	
;	jmp 0x08:gdt_flush
;gdt_flush:
;	mov ax, 10h
;	mov ds, ax
;	mov es, ax
;	mov fs, ax
;	mov gs, ax
;	mov ss, ax
;	;mov esp, 0x9C000
;	
;	ret
;	
global _gdt_load
;extern _gdtr
global _gdt32
_gdt32:
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
	; GDT Entry
	dd 0
	dd 0
	
_gdtr:
	dw 255
	dd _gdt32
_gdt_load:
	;mov   AX, [ESP + 4]
	;mov   BX, [ESP + 8]
	;mov   CL, [ESP + 12]
	;mov   DL, [ESP + 16]
	;mov [_gdt32+40], AX
	;mov [_gdt32+42], BX
	;mov [_gdt32+44], CL
	;mov [_gdt32+47], DL
	lgdt [_gdtr]
	
	jmp 0x08:gdt_flush
gdt_flush:
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	;mov esp, 0x9C000
	
	ret
	
;global _gdt_load
;_gdtr:
;	dw 0x9F00
;	dw 255
;_gdt_load:
;	lgdt [_gdtr]
;	
;	mov eax, _gdtr
;	mov ebx, [_gdtr]
;	mov ecx, [_gdtr+2]
;	mov edi, dword [0x9F08]
;	mov esi, dword [0x9F0C]
;	jmp 0x08:gdt_flush
;gdt_flush:
;	mov ax, 10h
;	mov ds, ax
;	mov es, ax
;	mov fs, ax
;	mov gs, ax
;	mov ss, ax
;	
;	ret