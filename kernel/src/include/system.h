
#include <stdint.h>

#ifndef $_SYSTEM_H
#define $_SYSTEM_H

typedef struct tag_regs16 regs16_t;
struct tag_regs16 
{
    uint16_t di, si, sp, bp, bx, dx, cx, ax;
    uint16_t ds, es, fs, gs, ss;
} __attribute__((packed));

typedef struct tag_regs32 regs32_t;
struct tag_regs32
{
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp;
    uint32_t eip, eflags, cr3;
} ;

extern void _sys_enableinterrupts();
extern void _sys_disableinterrupts();
extern void _sys_v86mode(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);
extern regs16_t *_sys_bioscalls32(uint8_t intnum, regs16_t *regs);
extern void _sys_enablepaging(uint32_t pagedir_address);

#endif