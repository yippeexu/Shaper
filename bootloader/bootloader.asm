BITS 16

jmp short start

iOEM db "ShapeOS"
iSectSize dw 0x200 ; bytes / sector
iClustSize db 1    ; 1 sector per cluster (for simplicity)
iResCnt dw 1       ; number of reserved sectors
iFatCnt db 2       ; # of fat copies
iRootSize dw 224   ; size of root dir
iTotalSect dw 2880 ; total sectors
iMedia db 0xF0     ; media descriptor
iFatSize dw 9      ; size of each FAT
iTrackSect dw 9    ; sectors per track
iHeadCnt dw 2      ; number of r/w heads
iHiddentSect dd 0  ; number of hidden sectors
iSect32 dd 0       ; number of > 32MB sectors
iBootDrive db 0    ; holds drive of bootsector
iReserved db 0     ; empty reserved attribute
iBootSign db 0x29  ; extended bootsig
iVolID db "seri"   ; disk serial
acVolumeLabel db "MYVOLUME  " ; volume label
acFSType db "FAT16  "         ; fs type

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
