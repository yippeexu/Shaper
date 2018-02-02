global _tss_load

_tss_load:
	mov ax, [esp+4]
	ltr ax
	ret