
#include <stdint.h>
#include <pci.h>

#ifndef $_ATA_H
#define $_ATA_H

uint16_t IO_ATA_DATA;
uint16_t IO_ATA_FEATURES;
uint16_t IO_ATA_SECTORS;
uint16_t IO_ATA_LBA0;
uint16_t IO_ATA_LBA1;
uint16_t IO_ATA_LBA2;
uint16_t IO_ATA_DRIVE;
uint16_t IO_ATA_COMMAND;
uint16_t IO_ATA_STATUS;
uint16_t IO_ATA_ALTSTATUS;

typedef enum
{
    ATA_CMD_IDENTIFY = 0xEC,
    ATA_CMD_FLUSH_CACHE = 0xE7,
    ATA_CMD_FLUSH_CACHE_EXT = 0xEA,
    ATA_CMD_READ_PIO = 0x20,
    ATA_CMD_WRITE_PIO = 0x30,
    ATA_CMD_READ_PIO_EXT = 0x24,
    ATA_CMD_WRITE_PIO_EXT = 0x34,
    ATA_CMD_READ_DMA = 0xC8,
    ATA_CMD_WRITE_DMA = 0xCA,
    ATA_CMD_READ_DMA_EXT = 0x25,
    ATA_CMD_WRITE_DMA_EXT = 0x35,
    ATA_CMD_PACKET = 0xA0,
    ATA_CMD_READ = 0xA8,
    ATA_CMD_WRITE = 0xAA,
    ATA_CMD_IDENTIFY_PACKET = 0xA1
} atacmd_t;

typedef enum
{
    ATA_DRIVE_NULL,
    ATA_DRIVE_NOTHING,
    ATA_DRIVE_ATA,
    ATA_DRIVE_ATAPI
} ata_drive_t;

void ata_init(pcidevice_t *ata_device, bool secondary, bool slave);
void ata_getports(bool secondary);

#endif