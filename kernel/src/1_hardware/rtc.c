
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <system.h>

#include <cmos.h>
#include <acpi.h>
#include <irq.h>
#include <pic.h>

#define bcd2int(bcd) ((bcd / 16) * 10) + (bcd & 0xf)

uint8_t creg;
void rtc_handler(x86_regs *context);

void rtc_init()
{
    // Enable IRQ 8
    cmos_write(0x0B, cmos_read(0x0B, true) |  0x40, true);

    // Shaper's RTC Configuration
    cmos_write(0x0B, cmos_read(0x0B, true) & ~0x02, true);
    cmos_write(0x0B, cmos_read(0x0B, true) &  0x04, true);
    
    cmos_read(0x0C, false);

    // Century register
    creg = acpi_get_century_reg();

    _irq_unmask(0x08);
    _irq_add_handler(0x08, rtc_handler);
}

uint8_t *rtc_get_current_datetime()
{
    while (cmos_read(0x0A, true) & 0x80) ;

    uint8_t datetime[8];
    datetime[0] = cmos_read(0x00, true);
    datetime[1] = cmos_read(0x02, true);
    datetime[2] = cmos_read(0x04, true);
    datetime[3] = cmos_read(0x08, true);
    datetime[4] = cmos_read(0x09, true);
    datetime[5] = cmos_read(creg, true);
    datetime[6] = cmos_read(0x06, true);
    datetime[7] = cmos_read(0x07, true);

    return datetime;
}

void rtc_handler(x86_regs *context)
{
}