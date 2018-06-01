global _tss_ltr
global _tss_str

_tss_ltr:
	mov ax, [esp+4]
	ltr ax

	ret

_tss_str:
	mov ax, [esp+4]
	str ax

	ret