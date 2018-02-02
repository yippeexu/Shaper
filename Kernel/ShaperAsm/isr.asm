global _isr0 ; Divide-by-zero (Useful for testing isrs)
global _isr1 ; Debug
global _isr2 ; Non-maskable interrupt
global _isr3 ; Breakpoint
global _isr4 ; Overflow
global _isr5 ; Bound Range Exceeded
global _isr6 ; Invaild Opcode
global _isr7 ; Device Not Available
global _isr8 ; Double Fault
global _isr9 ; Coproccessor Segment Overrun
global _isr10 ; Invaild TSS
global _isr11 ; Segment Not Present
global _isr12 ; Stack-Segment Fault
global _isr13 ; GPF (General Protection Fault)
global _isr14 ; Page Fault
global _isr15 ; Reserved (not an exception)
global _isr16 ; x87 Floating-Point Exception
global _isr17 ; Alignment Check
global _isr18 ; Machine Check
global _isr19 ; SMID Floating-Point Exception
global _isr20 ; Virtualization Exception
global _isr21 ; Reserved (not an exception)
global _isr22 ; Reserved (not an exception)
global _isr23 ; Reserved (not an exception)
global _isr24 ; Reserved (not an exception)
global _isr25 ; Reserved (not an exception)
global _isr26 ; Reserved (not an exception)
global _isr27 ; Reserved (not an exception)
global _isr28 ; Reserved (not an exception)
global _isr29 ; Reserved (not an exception)
global _isr30 ; Security Exception
global _isr31 ; Reserved (not an exception)

_isr0:
	cli
	push byte 0
	push byte 0
	jmp _isr_common_stub
	
_isr1:
	cli
	push byte 0
	push byte 1
	jmp _isr_common_stub
	
_isr2:
	cli
	push byte 0
	push byte 2
	jmp _isr_common_stub
	
_isr3:
	cli
	push byte 0
	push byte 3
	jmp _isr_common_stub
	
_isr4:
	cli
	push byte 0
	push byte 4
	jmp _isr_common_stub
	
_isr5:
	cli
	push byte 0
	push byte 5
	jmp _isr_common_stub
	
_isr6:
	cli
	push byte 0
	push byte 6
	jmp _isr_common_stub
	
_isr7:
	cli
	push byte 0
	push byte 7
	jmp _isr_common_stub
	
_isr8:
	cli
	push byte 0
	push byte 8
	jmp _isr_common_stub
	
_isr9:
	cli
	push byte 0
	push byte 9
	jmp _isr_common_stub
	
_isr10:
	cli
	push byte 10
	jmp _isr_common_stub
	
_isr11:
	cli
	push byte 11
	jmp _isr_common_stub
	
_isr12:
	cli
	push byte 12
	jmp _isr_common_stub
	
_isr13:
	cli
	push byte 13
	jmp _isr_common_stub
	
_isr14:
	cli
	push byte 14
	jmp _isr_common_stub
	
_isr15:
	cli
	push byte 0
	push byte 15
	jmp _isr_common_stub
	
_isr16:
	cli
	push byte 0
	push byte 16
	jmp _isr_common_stub
	
_isr17:
	cli
	push byte 17
	jmp _isr_common_stub
	
_isr18:
	cli
	push byte 0
	push byte 18
	jmp _isr_common_stub
	
_isr19:
	cli
	push byte 0
	push byte 19
	jmp _isr_common_stub
	
_isr20:
	cli
	push byte 0
	push byte 20
	jmp _isr_common_stub
	
_isr21:
	cli
	push byte 0
	push byte 21
	jmp _isr_common_stub
	
_isr22:
	cli
	push byte 0
	push byte 22
	jmp _isr_common_stub
	
_isr23:
	cli
	push byte 0
	push byte 23
	jmp _isr_common_stub
	
_isr24:
	cli
	push byte 0
	push byte 24
	jmp _isr_common_stub
	
_isr25:
	cli
	push byte 0
	push byte 25
	jmp _isr_common_stub
	
_isr26:
	cli
	push byte 0
	push byte 26
	jmp _isr_common_stub
	
_isr27:
	cli
	push byte 0
	push byte 27
	jmp _isr_common_stub
	
_isr28:
	cli
	push byte 0
	push byte 28
	jmp _isr_common_stub
	
_isr29:
	cli
	push byte 0
	push byte 29
	jmp _isr_common_stub
	
_isr30:
	cli
	push byte 30
	jmp _isr_common_stub
	
_isr31:
	cli
	push byte 0
	push byte 31
	jmp _isr_common_stub
	
extern _isr_fault_handler
_isr_common_stub:
	pusha
	push DS
	push ES
	push FS
	push GS
	
	mov AX, 10h
	mov DS, AX
	mov ES, AX
	mov FS, AX
	mov GS, AX
	mov EAX, ESP
	
	push EAX
	mov EAX, _isr_fault_handler
	call EAX
	
	popa
	pop DS
	pop ES
	pop FS
	pop GS
	
	pop EAX
	add ESP, 8
	iret