BITS 16

start:
    cli
	mov ax, 0x07C0	
	add ax, 288		
	mov ss, ax
	mov sp, 4096

	mov ax, 0x07C0		
	mov ds, ax
    sti

    mov ah, 0x06
    xor al, al
    xor cx, cx
    mov dx, 184FH
    mov bh, 0x1E
    int 0x10

	mov si, intro
	call puts16
    mov si, loadmsg
    call puts16
    mov si, failed
    call puts16

	jmp $			


	intro db 'Shaper Bootloader, written by KingLuigi4932 and Kerndever', 0xD, 0xA, 0
    loadmsg db 'Loading kernel...', 0
    failed db 'Failed!', 0xD, 0xA, 0


puts16:
	mov ah, 0Eh		

.repeat:
	lodsb			
	cmp al, 0
	je .done		
	int 10h			
	jmp .repeat

.done:
	ret


times 510-($-$$) db 0
dw 0xAA55	
