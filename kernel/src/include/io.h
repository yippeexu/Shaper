
#include "stdint.h"

#ifndef $_IO_H
#define $_IO_H

#define IO_ATA_PRIMARY_DATA 0x1F0
#define IO_ATA_PRIMARY_FEATURES 0x1F1
#define IO_ATA_PRIMARY_SECTORS 0x1F2
#define IO_ATA_PRIMARY_LBA0 0x1F3
#define IO_ATA_PRIMARY_LBA1 0x1F4
#define IO_ATA_PRIMARY_LBA2 0x1F5
#define IO_ATA_PRIMARY_DRIVE 0x1F6
#define IO_ATA_PRIMARY_COMMAND 0x1F7
#define IO_ATA_PRIMARY_STATUS 0x1F7
#define IO_ATA_PRIMARY_ALTSTATUS 0x3F6
#define IO_ATA_SECONDARY_DATA 0x170
#define IO_ATA_SECONDARY_FEATURES 0x171
#define IO_ATA_SECONDARY_SECTORS 0x172
#define IO_ATA_SECONDARY_LBA0 0x173
#define IO_ATA_SECONDARY_LBA1 0x174
#define IO_ATA_SECONDARY_LBA2 0x175
#define IO_ATA_SECONDARY_DRIVE 0x176
#define IO_ATA_SECONDARY_COMMAND 0x177
#define IO_ATA_SECONDARY_STATUS 0x177
#define IO_ATA_SECONDARY_ALTSTATUS 0x376
#define IO_FLP_STATUS_A 0x3F0
#define IO_FLP_STATUS_B 0x3F1
#define IO_FLP_OUTPUT 0x3F2
#define IO_FLP_TAPE_DRIVE 0x3F3
#define IO_FLP_MAIN_STATUS 0x3F4
#define IO_FLP_DATARATE_SEL 0x3F4
#define IO_FLP_DATA_FIFO 0x3F5
#define IO_FLP_INPUT 0x3F7
#define IO_FLP_CONFIG_CONTROL 0x3F7

extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);
extern uint32_t inl(uint16_t port);
extern void insb(uint16_t port, uint8_t *dest, uint32_t length);
extern void insw(uint16_t port, uint16_t *dest, uint32_t length);
extern void insl(uint16_t port, uint32_t *dest, uint32_t length);
extern void outb(uint16_t port, uint8_t value);
extern void outw(uint16_t port, uint16_t value);
extern void outl(uint16_t port, uint32_t value);
extern void outsb(uint16_t port, uint8_t* value, uint32_t length);
extern void outsw(uint16_t port, uint16_t* value, uint32_t length);
extern void outsl(uint16_t port, uint32_t* value, uint32_t length);
extern void io_sleep();
    
#endif