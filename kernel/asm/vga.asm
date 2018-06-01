
global video_interrupt

video_interrupt:
	db 0xCF
	mov ah, 00h
	mov al, 13h
	int 10h

	jmp $