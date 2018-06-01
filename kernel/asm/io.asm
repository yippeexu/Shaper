global inb
global inw
global inl
global insb
global insw
global insl
global outb
global outw
global outl
global outsb
global outsw
global outsl
global io_sleep

inb:
    xor al, al
    mov dx, [esp+4]
    in al, dx
    ret

inw:
    xor ax, ax
    mov dx, [esp+4]
    in ax, dx  
    ret

inl:
    xor eax, eax
    mov dx, [esp+4]
    in eax, dx
    ret

outb:
    mov al, [esp+8]
    mov dx, [esp+4]
    out dx, al
    ret

outw:
    mov ax, [esp+8]
    mov dx, [esp+4]
    out dx, ax
    ret

outl:
    mov edi, [esp+8]
    mov dx, [esp+4]
    out dx, eax
    ret

insb:
    mov edi, [esp+8]
    mov dx, [esp+4]
    mov ecx, [esp+12]
    rep insb
    mov eax, edi
    ret

insw:
    mov edi, [esp+8]
    mov dx, [esp+4]
    mov ecx, [esp+12]
    rep insw
    mov eax, edi
    ret

insl:
    mov edi, [esp+8]
    mov dx, [esp+4]
    mov ecx, [esp+12]
    rep insd
    mov eax, edi
    ret

outsb:
    mov esi, [esp+8]
    mov dx, [esp+4]
    mov ecx, [esp+12]
    rep outsb
    ret

outsw:
    mov esi, [esp+8]
    mov dx, [esp+4]
    mov ecx, [esp+12]
    rep outsw
    ret

outsl:
    mov esi, [esp+8]
    mov dx, [esp+4]
    mov ecx, [esp+12]
    rep outsd
    ret

io_sleep:
    in al, 0x00
    ret