
#include <stdint.h>
#include <string.h>

#define big_endian16(x) (x & 0xFF) << 8 | (x >> 8) 
#define big_endian32(x) (x & 0xFF) << 24 | ((x >> 8) & 0xFF) << 16 | ((x >> 16) & 0xFF) << 8 | (x >> 24)

void change_endianness(char *string, size_t length);
