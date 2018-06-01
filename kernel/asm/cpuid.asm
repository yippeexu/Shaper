
global _cpuid_check
global _cpuid_get_vendorid
global _cpuid_get_features

_cpuid_check:
    pushfd
    pushfd
    xor dword [esp], 0x200000
    popfd
    pushfd
    pop eax
    xor eax, [esp]
    popfd
    and eax, 0x200000
    ret

_cpuid_get_vendorid:
    mov eax, 0x00
    cpuid

    mov eax, [esp+4]
    mov dword [eax], ebx
    mov dword [eax+4], edx
    mov dword [eax+8], ecx
    mov byte [eax+12], 0

    ret

_cpuid_get_features:
    mov eax, 0x01
    cpuid

    mov eax, [esp+4]
    mov ebx, [esp+8]

    .move_low:
        cmp eax, 0
        jz .move_high
        mov [eax], ecx
    .move_high:
        cmp ebx, 0
        jz .return
        mov [ebx], edx

    .return:
        ret