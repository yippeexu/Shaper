
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ata.h>

typedef struct tag_atapipacket atapi_packet_t;
struct tag_atapipacket
{
    atacmd_t ata_command;
    uint8_t is_relative : 1;
    uint8_t reserved0 : 4;
    uint8_t unused : 3;
    uint8_t lba3;
    uint8_t lba2;
    uint8_t lba1;
    uint8_t lba0;
    uint8_t reserved1[2];
    uint8_t pmi : 1;
    uint8_t reserved2 : 7;
    uint8_t control;
    uint8_t atapi_specific[2];
} __attribute__((packed));

void atapi_init(bool secondary, bool slave);
int atapi_read(uint64_t lba, uint64_t count, uint8_t* buffer);
int atapi_write(uint64_t lba, uint64_t count, uint8_t* buffer);