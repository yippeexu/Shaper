BITS 16
ORG 0x7C00

jmp short start
nop

;; [BIOS Parameter Block]
bpb:
iOEM db "SHAPEROS"
iSectSize dw 512 ; bytes / sector
iClustSize db 1    ; 1 sector per cluster (for simplicitty)
iResCnt dw 1       ; number of reserved sectors
iFatCnt db 2       ; # of fat copies
iRootSize dw 224   ; size of root dir
iTotalSect dw 2880 ; total sectors
iMedia db 0xF0     ; media descriptor
iFatSize dw 9      ; size of each FAT
iTrackSect dw 18    ; sectors per track
iHeadCnt dw 2      ; number of r/w heads
iHiddentSect dd 0  ; number of hidden sectors
iSect32 dd 0       ; number of > 32MB sectors
iBootDrive db 0    ; holds drive of bootsector
iReserved db 0     ; empty reserved attribute
iBootSign db 0x29  ; extended bootsig
iVolID dd 0x50001920           ; disk serial
acVolumeLabel db "SOS FLOPPY " ; volume label
acFSType db "FAT16  "          ; fs type

;; [Loader informations]
fileloader_name db 'LOADER  SSF'

start:
	mov [iBootDrive], dl

    cli
	
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	mov sp, 7C00h
    sti
	
	call clear_screen

	mov si, intro
	call puts16
	
	call bioscall_resetfloppy
	
    mov si, loadmsg
    call puts16

	mov ax, 21h
	call logicalsector_to_chs
	
	jmp jump_to_loader
	
	; If unexpectdly returned to here then halt the cpu!

halt:
	hlt
	jmp halt
	
	intro db 'Shaper Bootloader, written by KingLuigi4932 and Kerndever', 0xD, 0xA, 0
    loadmsg db 'Loading kernel... ', 0xD, 0xA, 0
    failed db 'Failed!', 0xD, 0xA, 0

;; [Floppy functions] -------
bioscall_resetfloppy:
	mov ah, 00h
	mov dl, [iBootDrive]
	int 13h
	
	ret
	
jump_to_loader:
	mov ah, 02h
	mov al, 01h
	mov dl, [iBootDrive]
	mov bx, 0100h
	mov es, bx
	mov bx, 00h
	int 13h
	
	jc print_error
	
	mov dl, [iBootDrive]
	jmp 1000h
	
	ret
	
;; [Utils] -----------------------

;; Special Thanks to Mike Saunders and MikeOS Developers
;;; LBA to CHS Conversion
;;; INPUT: AX = LBA | OUTPUT: CH = CYLINDER, DH = HEAD, CL = SECTOR
logicalsector_to_chs:
	
	mov bx, ax			; Save logical sector

	mov dx, 0			; First the sector
	div word [iTrackSect]
	add dl, 01h			; Physical sectors start at 1
	mov cl, dl			; Sectors belong in CL for int 13h
	mov ax, bx

	mov dx, 0			; Now calculate the head
	div word [iTrackSect]
	mov dx, 0
	div word [iHeadCnt]
	mov dh, dl			; Head/side
	mov ch, al			; Track

	mov dl, byte [iBootDrive]		; Set correct device

	ret
	
;; [Print functions] -------------
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
	
print_error:
    mov si, failed
    call puts16
	
	jmp $

clear_screen:
	; Clear screen
	;; Set mode to clear screen for all bioses
	mov AH, 00h
	mov AL, 02h ; AL=02 (80x25 text mode)
	int 10h ; BIOS style: Video calls
	
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

diskbuffer:
