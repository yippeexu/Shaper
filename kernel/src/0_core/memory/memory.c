
#include <stdint.h>

uint32_t get_ebda_base_address()
{
    uint16_t *bd = (uint16_t *)0x40E;
    return (*(bd) >> 4) << 8;
}