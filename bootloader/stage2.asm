BITS 16
ORG 0x9C00

start:
	cli
	call a20_enable
	call gdt_load
	call pmode_enter

	jmp 0x08:bmain32

a20_enable: ; a20 line fast gate
	in al, 0x92
	or al, 2
	out 0x92, al

pmode_enter:
	mov eax, cr0
	or al, 1
	mov cr0, eax

gdt_load:
	lgdt[toc]
	ret

; thanks brokenthorn osdev tutorial
gdt_data: 
	dd 0 				; null descriptor
	dd 0 
 
; gdt code:				; code descriptor
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 10011010b 			; access
	db 11001111b 			; granularity
	db 0 				; base high
 					; data descriptor
	dw 0FFFFh 			; limit low (Same as code)
	dw 0 				; base low
	db 0 				; base middle
	db 10010010b 			; access
	db 11001111b 			; granularity
	db 0				; base high
 
end_of_gdt:
toc: 
	dw end_of_gdt - gdt_data - 1 	; limit (Size of GDT)
	dd gdt_data 			; base of GDT

BITS 32

bmain32:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 0x9C00

	mov eax, 0xDEADBEEF
