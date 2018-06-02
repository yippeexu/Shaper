
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <pci.h>

#include <blockdevice.h>

void atapio_init(bool secondary, bool slave);
bool atapio_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t* buffer);
bool atapio_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t* buffer);

