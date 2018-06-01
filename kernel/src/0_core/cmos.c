
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <system.h>

#include <io.h>

uint8_t cmos_read(uint8_t reg_num, bool disable_nmi)
{
    _sys_disableinterrupts();
    outb(0x70, reg_num | (disable_nmi << 7));
    uint8_t value = inb(0x71);
    _sys_enableinterrupts();
    return value;
}

void cmos_write(uint8_t reg_num, uint8_t value, bool disable_nmi)
{
    _sys_disableinterrupts();
    outb(0x70, reg_num | (disable_nmi << 7));
    outb(0x71, value);
    _sys_enableinterrupts();
}