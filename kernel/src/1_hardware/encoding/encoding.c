
#include <stdint.h>
#include <string.h>

void change_endianness(char *endianness_string, size_t length)
{
    uint16_t* chars = (uint16_t *)endianness_string;
    for (int i = 0; i < length / 2; i++)
        chars[i] = (chars[i] & 0xFF) << 8 | ((chars[i] >> 8) & 0xFF);
    chars[length] = '\0';
}
