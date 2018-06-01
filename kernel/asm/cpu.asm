
global _get_msr
global _set_msr

_set_msr:
    mov ecx, [esp+4]
    mov eax, [esp+8]
    mov edx, [esp+12]

    wrmsr

    ret

_get_msr:
    mov ecx, [esp+4]
    mov ebp, [esp+8]
    mov ebx, [esp+12]

    rdmsr

    mov [ebp], eax
    mov [ebx], edx

    ret