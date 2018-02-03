BITS 16
ORG 0x7E00

; needed stuff
iHeadCnt dw 2
iTrackSect dw 18
iBootDrive db 0

start:
	; boot drive is really important or some slots may not let the bootloader work
	mov [iBootDrive], dl

	; load kernel
	call load_kernel
	
	; to protected mode
	cli
	call a20_enable
	call gdt_load
	call pmode_enter

	; jump to kernel
	mov ax, 0x20
	jmp 0x18:bmain32
	;jmp kmain32
	
load_kernel:
	call load_rootdir
	call get_kernel
	call load_kernel_sectors
	
	ret
	
load_rootdir:
	mov ah, 02h
	mov al, 0Eh
	mov ch, 00h
	mov cl, 02h ; |
	mov dh, 01h ; | sector 0x13 (root dir sector)
	mov dl, [iBootDrive]
	mov bx, 0A000h
	int 13h
	
	jc puts16_error
	
	ret
	
get_kernel:
	mov di, 0A000h
	mov si, kernel_fat32_filename
	
.loop:
	; check if reached it limits
   cmp di, (0xA000 + (0x20 * 224))
   je puts16_error

   mov al, [si]
   mov bl, [di]
   cmp al, bl
   jne .next
   
   cmp al, [kernel_fat32_filename+0Ah]
   je .found
  
   inc di
   inc si
   jmp .loop ; next character
   
   .next:
	add di, 20h
	jmp .loop
	
   .found:
	mov bp, di
	sub bp, 0Ah
	
	; little endian assignment
	 ; starting cluster
	mov cl, [bp+26]
	mov ch, [bp+27]
	 ; size
	mov dl, [bp+28]
	mov dh, [bp+29]

	mov [kernel_starting_cluster], cx 
	mov [kernel_file_size], dx
	
	 ; amount of clusters
	mov ax, dx
	xor dx, dx
	mov cx, 200h
	div cx
	inc ax ; increment one cluster because (floating numbers even if they are greater than (x.4) still the reuslt is (x))
	
	mov [kernel_clusters], ax
	
	ret
	
load_kernel_sectors:
	mov ax, 1Fh
	add ax, word [kernel_starting_cluster]
	call ls2chs
	
	mov ah, 02h
	mov al, byte [kernel_clusters]
	mov dl, [iBootDrive]
	
	; 0x10000 = 1000h:0h
	mov bx, 1000h
	mov es, bx
	mov bx, 0000h
	
	int 13h
	
	jc puts16_error
	
	ret

a20_enable: ; a20 line fast gate
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

pmode_enter:
	mov eax, cr0 
	or al, 1
	mov cr0, eax
	ret 

gdt_load:
	lgdt [gdtr]
	ret

; thanks brokenthorn osdev tutorial
gdt_data: 
	dd 0 				; null descriptor
	dd 0 
 
; gdt 16:				; code descriptor
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 10011010b 			; access
	db 10001111b 			; granularity
	db 0 				; base high
 					; data descriptor
	dw 0FFFFh 			; limit low (Same as code)
	dw 0 				; base low
	db 0 				; base middle
	db 10010010b 			; access
	db 10001111b 			; granularity
	db 0				; base high
 
; gdt 32:				; code descriptor
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 10011010b 			; access
	db 11001111b 			; granularity
	db 0 				; base high
 					; data descriptor
	dw 0FFFFh 			; limit low (Same as code)
	dw 0 				; base low
	db 0 				; base middle
	db 10010010b 			; access
	db 11001111b 			; granularity
	db 0				; base high
 
gdt_data.end:
gdtr: 
	dw gdt_data.end - gdt_data - 1 	; limit (Size of GDT)
	dd gdt_data 			; base of GDT
	
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

puts16_magicerror:
	mov si, failed_msg
	call puts16
	
	mov si, magicerror_msg
	call puts16
	
	mov si, anykey_msg
	call puts16
	jmp $
	
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
	
pointer dw 0
	
kernel_fat32_filename db 'SHAPER  BIN'

failed_msg db 'Failed!', 0xD, 0xA, 0
anykey_msg db 'Press any key to reboot...', 0
magicerror_msg db 'Invaild kernel!', 0xD, 0xA, 0

magic dw 0x5341, 0x4254

BITS 32
bmain32:
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	; Magic check doesn't work ..?
	;mov ecx, [kernel_file_size]
	;mov di, 0x10000
	;add di, cx
	;sub di, 4
	;mov si, magic
	;mov cx, 4
	;rep cmpsb
	;jne puts16_magicerror
	
	mov esp, 0x10000
	mov eax, 0x10000
	jmp eax
	
BITS 16
times 768-($-$$) db 7Fh
kernel_starting_cluster dw 0
kernel_file_size dw 0
kernel_clusters dw 0 ; calculated (file_size / 200h + 1 for avoiding floating point values)
