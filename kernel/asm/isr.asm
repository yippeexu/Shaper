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
	push dword 0
	push dword 0
	jmp _isr_common_stub
	
_isr1:
	cli
	push dword 0
	push dword 1
	jmp _isr_common_stub
	
_isr2:
	cli
	push dword 0
	push dword 2
	jmp _isr_common_stub
	
_isr3:
	cli
	push dword 0
	push dword 3
	jmp _isr_common_stub
	
_isr4:
	cli
	push dword 0
	push dword 4
	jmp _isr_common_stub
	
_isr5:
	cli
	push dword 0
	push dword 5
	jmp _isr_common_stub
	
_isr6:
	cli
	push dword 0
	push dword 6
	jmp _isr_common_stub
	
_isr7:
	cli
	push dword 0
	push dword 7
	jmp _isr_common_stub
	
_isr8:
	cli
	push dword 0
	push dword 8
	jmp _isr_common_stub
	
_isr9:
	cli
	push dword 0
	push dword 9
	jmp _isr_common_stub
	
_isr10:
	cli
	push dword 10
	jmp _isr_common_stub
	
_isr11:
	cli
	push dword 11
	jmp _isr_common_stub
	
_isr12:
	cli
	push dword 12
	jmp _isr_common_stub
	
_isr13:
	cli
	push dword 13
	jmp _isr_common_stub
	
_isr14:
	cli
	push dword 14
	jmp _isr_common_stub
	
_isr15:
	cli
	push dword 0
	push dword 15
	jmp _isr_common_stub
	
_isr16:
	cli
	push dword 0
	push dword 16
	jmp _isr_common_stub
	
_isr17:
	cli
	push dword 17
	jmp _isr_common_stub
	
_isr18:
	cli
	push dword 0
	push dword 18
	jmp _isr_common_stub
	
_isr19:
	cli
	push dword 0
	push dword 19
	jmp _isr_common_stub
	
_isr20:
	cli
	push dword 0
	push dword 20
	jmp _isr_common_stub
	
_isr21:
	cli
	push dword 0
	push dword 21
	jmp _isr_common_stub
	
_isr22:
	cli
	push dword 0
	push dword 22
	jmp _isr_common_stub
	
_isr23:
	cli
	push dword 0
	push dword 23
	jmp _isr_common_stub
	
_isr24:
	cli
	push dword 0
	push dword 24
	jmp _isr_common_stub
	
_isr25:
	cli
	push dword 0
	push dword 25
	jmp _isr_common_stub
	
_isr26:
	cli
	push dword 0
	push dword 26
	jmp _isr_common_stub
	
_isr27:
	cli
	push dword 0
	push dword 27
	jmp _isr_common_stub
	
_isr28:
	cli
	push dword 0
	push dword 28
	jmp _isr_common_stub
	
_isr29:
	cli
	push dword 0
	push dword 29
	jmp _isr_common_stub
	
_isr30:
	cli
	push dword 30
	jmp _isr_common_stub
	
_isr31:
	cli
	push dword 0
	push dword 31
	jmp _isr_common_stub
	
;_isr0:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr1:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $
;		
;_isr2:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr3:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr4:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr5:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $
;		
;_isr6:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr7:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr8:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr9:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr10:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr11:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr12:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr13:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr14:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr15:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr16:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr17:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr18:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr19:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr20:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr21:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $
;		
;_isr22:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr23:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr24:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr25:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $
;		
;_isr26:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr27:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr28:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $
;		
;_isr29:	  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr30:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	
;		
;_isr31:  
;	mov eax, 0xDEADC0DE
;    mov DWORD [0xb8000], eax
;    ;cli
;    jmp $	

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
	
	pop EAX
	
	pop GS
	pop FS
	pop ES
	pop DS
	popa
	
	add ESP, 8
	iret