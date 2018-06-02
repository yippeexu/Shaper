
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <heap.h>
//#include <shapervfs.h>

#ifndef $_BLOCKDEV_H
#define $_BLOCKDEV_H

typedef struct tag_blockdevice blockdevice_t;
struct tag_blockdevice
{ 
    //uint64_t sectors_count;
    //uint32_t bytes_per_sector;
    char signature[6]; // 'BLKDEV'
    char dev_signature[4]; // 'SATA', 'ATA ', 'PART'
    char model_name[40]; 
    char firmware_rev[8];
    char serial_no[20];
    int (*read_sector)(blockdevice_t *, uint32_t, uint32_t, uint8_t *);
    int (*write_sector)(blockdevice_t *, uint32_t, uint32_t, uint8_t *);
    uint32_t *dev_specifics;
    bool is_controller;
    bool is_packet;
} __attribute__((packed));

blockdevice_t *blockdevices[256];

void add_blockdevice(blockdevice_t *block_device);

#endif