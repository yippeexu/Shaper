
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void cpuid_init();

extern bool _cpuid_check();
extern void _cpuid_get_vendorid(char *str);
extern void _cpuid_get_features(uint32_t *feature_low, uint32_t *feature_high);