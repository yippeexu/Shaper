global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

_irq0:
	cli
	push dword 0
	push dword 32
	jmp _irq_common_stub

_irq1:
	cli
	push dword 0
	push dword 33
	jmp _irq_common_stub

_irq2:
	cli
	push dword 0
	push dword 34
	jmp _irq_common_stub

_irq3:
	cli
	push dword 0
	push dword 35
	jmp _irq_common_stub

_irq4:
	cli
	push dword 0
	push dword 36
	jmp _irq_common_stub

_irq5:
	cli
	push dword 0
	push dword 37
	jmp _irq_common_stub

_irq6:
	cli
	push dword 0
	push dword 38
	jmp _irq_common_stub

_irq7:
	cli
	push dword 0
	push dword 39
	jmp _irq_common_stub

_irq8:
	cli
	push dword 0
	push dword 40
	jmp _irq_common_stub

_irq9:
	cli
	push dword 0
	push dword 41
	jmp _irq_common_stub

_irq10:
	cli
	push dword 0
	push dword 42
	jmp _irq_common_stub

_irq11:
	cli
	push dword 0
	push dword 43
	jmp _irq_common_stub

_irq12:
	cli
	push dword 0
	push dword 44
	jmp _irq_common_stub

_irq13:
	cli
	push dword 0
	push dword 45
	jmp _irq_common_stub

_irq14:
	cli
	push dword 0
	push dword 46
	jmp _irq_common_stub

_irq15:
	cli
	push dword 0
	push dword 47
	jmp _irq_common_stub
	
extern irq_handler

_irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret