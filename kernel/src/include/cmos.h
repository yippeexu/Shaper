
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

uint8_t cmos_read(uint8_t reg_num, bool disable_nmi);
void cmos_write(uint8_t reg_num, uint8_t value, bool disable_nmi);