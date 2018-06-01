
global asm_memcpy
global asm_memset

asm_memcpy:
    cld
    mov edi, [esp+4]
    mov esi, [esp+8]
    mov ecx, [esp+12]
    rep movsb

    ret

asm_memset:
    cld
    mov edi, [esp+4]
    mov eax, [esp+8]
    mov ecx, [esp+12]
    rep movsd

    ret