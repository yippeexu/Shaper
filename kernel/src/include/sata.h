
#include <stdint.h>

#include <ahci.h>

void sata_init(ahci_generic_t *ahci_controller_mem, ahci_port_t *sata_port, int port_number);
bool sata_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);
bool sata_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);
