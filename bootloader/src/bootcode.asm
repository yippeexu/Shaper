BITS 16
ORG 0x7C00

jmp short start
nop

; [BIOS Parameter Block]
bpb:
iOEM db "SHAPEROS"
iSectSize dw 512 ; bytes / sector
iClustSize db 1    ; 1 sector per cluster (for simplicitty)
iResCnt dw 1       ; number of reserved sectors
iFatCnt db 2       ; # of fat copies
iRootSize dw 224   ; size of root dir
iTotalSect dw 2880 ; total sectors (size of floppy (in bytes) / bytes per sector)
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
	
	jmp execute_loader
	
; if unexpectdly returned to here then halt the cpu!
halt:
	hlt
	jmp halt
	
intro db 'Shaper Bootloader, written by KingLuigi4932 and Kerndever', 0xD, 0xA, 0
loadmsg db 'Loading kernel... ', 0
failed_msg db 'Failed!', 0xD, 0xA, 0
anykey_msg db 'Press any key to reboot!', 0

; floppy functions
bioscall_resetfloppy:
	mov ah, 00h
	mov dl, [iBootDrive]
	int 13h
	
	jc puts16_error
	
	ret
	
jump_to_loader:
	mov dl, [iBootDrive]
	mov byte [7E04h], dl
	jmp 7E05h
	
	ret
	
	
execute_loader:
	call load_rootdir
	call get_file_loader
	call load_file_loader_sectors
	call jump_to_loader
	
	ret
	
load_rootdir:
	mov ah, 02h
	mov al, 0Eh
	mov ch, 00h
	mov cl, 02h ; |
	mov dh, 01h ; | sector 0x13 (root dir sector)
	mov dl, [iBootDrive]
	mov bx, 0A00h
	mov es, bx
	mov bx, 0000h
	int 13h
	
	jc puts16_error
	
	ret
	;jmp $
	
get_file_loader:
	;push ax
	mov di, 0A000h
	mov si, loader_fat32_filename
	
.loop:
	; check if reached it limits
   cmp di, (0xA000 + (0x20 * 224))
   je puts16_error

   mov al, [si]
   mov bl, [di]
   cmp al, bl
   jne .next
   
   cmp al, 0
   je .found
  
   inc di
   inc si
   jmp .loop ; loop!
   
   .next:
	add di, 20h
	jmp .loop
	
   .found:
	mov bp, di
	sub bp, 2Bh
	
	; little endian assignment
	 ; starting cluster
	mov cl, [bp+26]
	mov ch, [bp+27]
	 ; size
	mov dl, [bp+28]
	mov dh, [bp+29]
	
	mov [loader_starting_cluster], cx 
	mov [loader_file_size], dx
	
	;jmp $
	
	;pop cx 
	;pop dx
	
	 ; amount of clusters
	mov ax, dx
	xor dx, dx
	mov cx, 200h
	div cx
	inc ax ; increment one cluster because (floating numbers even if they are greater than (x.4) still the reuslt is (x))
	
	mov [loader_clusters], ax
	
	ret
	
load_file_loader_sectors:
	mov ax, 1Fh
	add ax, word [loader_starting_cluster]
	call ls2chs

	mov ah, 02h
	mov al, byte [loader_clusters]
	mov bx, 07E0h
	mov es, bx
	mov bx, 0
	mov dl, [iBootDrive]
	int 13h
	
	jc puts16_error
	
	ret ;jump_to_loader
	
; utils
; credits: mike saunders and mikeos developers
ls2chs:
	
	mov bx, ax			; save logical sector

	mov dx, 0			; first the sector
	div word [iTrackSect]
	add dl, 01h			; physical sectors start at 1
	mov cl, dl			; sectors belong in CL for int 13h
	mov ax, bx

	mov dx, 0			; Now calculate the head
	div word [iTrackSect]
	mov dx, 0
	div word [iHeadCnt]
	mov dh, dl			; head/side
	mov ch, al			; track

	mov dl, byte [iBootDrive]		; set correct device

	ret
	
; print functions
puts16_error:
	mov si, failed_msg
	call puts16
	
	mov si, anykey_msg
	call puts16
	
	mov ah, 00h
	int 16h
	
	; jmp word 0xFFFF:0x0000
    db 0xEA 
    dw 0x0000 
    dw 0xFFFF

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
	; clear screen
	 ; set mode to clear screen for all bioses
	mov AH, 00h
	mov AL, 02h ; AL=02 (80x25 text mode)
	int 10h
	
	 ; fill screen with blue background
	mov AH, 09h
	mov AL, 20h
	mov BH, 00h
	mov BL, 1Eh
	mov CX, 2000h
	int 10h
	
	ret
	
loader_fat32_filename db 'SHAPER  BIN'
loader_starting_cluster dw 0
loader_file_size dw 0
loader_clusters db 0

; boot signature
times 510-($-$$) db 0
dw 0xAA55