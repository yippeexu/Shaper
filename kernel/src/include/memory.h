
#include <stdint.h>

extern void _set_msr(uint32_t msr, uint32_t low, uint32_t high);
extern void _get_msr(uint32_t msr, uint32_t *low, uint32_t *high);

uint32_t get_ebda_base_address();