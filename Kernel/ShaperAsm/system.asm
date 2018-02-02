global _sys_enableinterrupts
global _sys_disableinterrupts
global _sys_v86mode
global _sys_realmode
global _sys_enablepaging
global blinking_fix

extern init1
extern printnum

backup_ebx dd 0

_sys_disableinterrupts:
	cli
	ret
_sys_enableinterrupts:
	sti
	;call _sys_v86mode
	ret

asm_memcopy:
	mov eax, [ebp+ebx]
	stosb
	inc ebx
	
	dec ecx
	jnz asm_memcopy
	ret
	
_sys_v86mode:
	mov edi, 0x9000
	mov ebp, blinking_fix
	;mov [backup_ebx], ebx
	mov ebx, 0h
	mov ecx, (blinking_fix_length - blinking_fix)
	jmp asm_memcopy
	;mov ebx, 0xB8000
	;mov ebx, [backup_ebx]
	
	cli
	cld
	
	;mov ebp, esp
	
	;push  0h        
	;push  9000h        
	
	;pushfd
	;or dword [esp], (1 << 17)  
	
	;push 0        
	;push 9000h       
	
	;iretd
	
	;mov cs, [ebp+12]
	;mov eax, 9000h
	;jmp eax
	
    ;cli
    ;cld

    ; put a "jmp $" instruction at 0x0800:0x1000
    ;mov edi, 0x9000
	;mov al, 
	;mov ebp, blinking_fix
	;mov ecx, 0x10
	;mov ebx, 0x00
	;jmp asm_memcopy
	; mov ax, 0x4f02
	;mov al, 0x66
	;stosb
	;mov al, 0xB8
	;stosb
	;mov al, 0x02
	;stosb
	;mov al, 0x4F
	;stosb
	;
	;; mov bx, 0x411d
	;mov al, 0x66
	;stosb
	;mov al, 0xbb
	;stosb
	;mov al, 0x1d
	;stosb
	;mov al, 0x41
	;stosb
	;
	;; int 0x10
	;mov al, 0xCD
	;stosb
	;mov al, 0x10
	;stosb
	;
	;; jmp $
	;mov al, 0xEB
	;stosb
	;mov al, 0xFE
	;stosb

    ; enter v8086 mode
    push 0x0800                ; SS
    push 0x1000            ; ESP - just for testing

    pushfd                ; EFLAGS
    pop eax                ; get back EFLAGS
    or eax, (1 << 17)        ; enable v8086 flag
    push eax            ; EFLAGS

    push 0x0800                ; CS
    push 0x1000			    	; EIP
	
	iretd
	
blinking_fix:
	mov ax, 4F02h
	mov bx, 411Dh
	int 10h
	;jmp $
	;call vm8086_entrance
blinking_fix_length:
	
_idtr_real:
	dw 0x3ff
	dd 0
	
;bits 16
_sys_realmode:
	;cli
	
	mov eax, 10h
	mov ds, eax
	;assume ds:nothing
	
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov ss, eax
	;assume es:nothing, fs:nothing, gs:nothing, ss:nothing
	
	mov eax, cr0
	and al, ~1
	mov cr0, eax
	
	;mov ax, 18h
	;mov ds, ax
	;assume ds:
	
	lidt [_idtr_real]
	
	jmp 0x0:finish_realmode
	
finish_realmode:
	mov sp, 0x8000

	;sti
	
	mov AX, 1003h
	mov BL, 00h
	int 10h
	
	call init1
	ret
	
_sys_enablepaging:
	mov eax, [esp+4]
	mov cr3, eax
	
	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax
	ret