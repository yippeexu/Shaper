global _idt_load
extern _idtr

_idt_load:
	; Move the base address to our idt_info structure
	;mov EAX, [ESP + 4]
	;mov [_idtr + 2], EAX
	cli
	
	lidt [_idtr]
	ret