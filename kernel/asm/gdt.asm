global _gdt_load
global _gdt32
global _gdtr

extern paging_init
section .rodata
	_gdt32:
		; GDT Entry
		dd 0
		dd 0
		
		; GDT Entry
		dw 0xFFFF
		dw 0
		db 0
		db 0x9A
		db 11001111b
		db 0
		
		; GDT Entry
		dw 0xFFFF
		dw 0
		db 0
		db 0x92
		db 11001111b
		db 0
		
		; GDT Entry
		dw 0xFFFF
		dw 0
		db 0
		db 0xFA
		db 11001111b
		db 0
		
		; GDT Entry
		dw 0xFFFF
		dw 0
		db 0
		db 0xF2
		db 11001111b
		db 0
		
		; GDT Entry
		dw 0xFFFF
		dw 0
		db 0
		db 0x9A
		db 10001111b
		db 0
		
		; GDT Entry
		dw 0xFFFF
		dw 0
		db 0
		db 0x92
		db 10001111b
		db 0
		
		; GDT Entry
		dw 0xFFFF
		dw 0
		db 0
		db 0xFA
		db 10001111b
		db 0
		
		; GDT Entry
		dw 0xFFFF
		dw 0
		db 0
		db 0xF2
		db 10001111b
		db 0
		
		; GDT Entry
		dd 0
		dd 0
		
		; GDT Entry
		dd 0
		dd 0
		
	_gdtr:
		dw $ - _gdt32 - 1
		dd _gdt32 - 0xC0100000 + 0x100000

section .text
	_gdt_load:
		lgdt [_gdtr - 0xC0100000 + 0x100000]
		
		jmp 0x08:(gdt_flush - 0xC0100000 + 0x100000)
	gdt_flush:
		mov ax, 10h
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		sub esp, 0xC0100000
		add esp, 0x100000

		call paging_init
		
		ret
		