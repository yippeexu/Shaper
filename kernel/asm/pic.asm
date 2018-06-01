global reset_pic

;; input: bh = pic master vector : bl = pic slave vector
reset_pic:
    push ax
    mov al, 0x11
    out 0x20, al
    out 0xA0, al
    mov al, bh
    out 0x21, al
    mov al, bl
    out 0xA1, al
    mov al, 0x04
    out 0x21, al
    shr al, 1
    out 0xA1, al
    shr al, 1
    out 0x21, al
    out 0xA1, al
    pop ax
    ret