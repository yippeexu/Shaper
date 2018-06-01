global _idt_load
extern _idtr

_idt_load:
	lidt [_idtr]
	ret