
#include <stdint.h>

void _pic_remap(int offset1, int mask1, int offset2, int mask2);
void _pic_sendEOI(uint8_t irq);
bool _pic_in_service(uint8_t irq);