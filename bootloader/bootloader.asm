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
	
	call clear_screen

	mov si, intro
	call puts16
    mov si, loadmsg
    call puts16
    mov si, failed
    call puts16

	jmp $			


	intro db 'Shaper Bootloader, written by KingLuigi4932 and Kerndever', 0xD, 0xA, 0
    loadmsg db 'Loading kernel... ', 0
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

clear_screen:
	mov AX, 1003h
	mov BL, 00h
	int 10h
	
	; Clear screen
	;; Set mode to clear screen for all bioses
	mov AH, 00h
	int 10h
	
	;; Fill screen with blue background
	mov AH, 09h
	mov AL, 20h
	mov BH, 00h
	mov BL, 1Eh
	mov CX, 2000h
	int 10h
	
	ret
	
times 510-($-$$) db 0
dw 0xAA55
