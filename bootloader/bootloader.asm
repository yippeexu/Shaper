;%include "kernel.asm"
org 0x7C00 

jmp bootintro

intro0 db "Shaper | Bootloader by KingLuigi4932 and Kerndever"
intro1 db "Loading kernel... "
error0 db "Error occured while loading Kernel!"
error1 db "Shaper.elf cannot be found!"
failed db "Failed!"
intro0_len db 33h
intro1_len db 12h
error0_len db 23h
error1_len db 1Bh
failed_len db 07h

;; [FAT12 Informations] ;;
cluster dw 0
pointer dw 0
;; -------------------- ;;

;; [File information] ;;
kernel_filename db "SHAPER  ELF"
;; ------------------ ;;
sys_print:
    mov ah, 13h
	mov al, 01h
	mov bh, 00h
    int 0x10
	ret

bootintro:
	cli
	mov AX, CS
	mov DS, AX
	mov ES, AX
	mov SS, AX
	mov SP, 9C00h	
	sti
	
	mov AX, 1003h
	mov BL, 00h
	int 10h
	
	mov AH, 09h
	mov AL, 20h
	mov BH, 00h
	mov BL, 0CEh
	mov CX, 2000
	int 10h

	mov BP, intro0
	mov BL, 0CEh
	mov CX, 32h
	mov DH, 0
	mov DL, 0
	call sys_print
	
	mov BP, intro1
	mov BL, 0CEh
	mov CX, 12h
	mov DH, 1
	mov DL, 0
	call sys_print
	
	mov BP, failed
	mov BL, 0ECh
	mov CX, 07h
	mov DH, 1
	mov DL, 12h
	call sys_print
	
halt:
	jmp halt

	times 510-($-$$) db 0
	dw 0xAA55
;disk_buffer:
