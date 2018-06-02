
#include <sysprint.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include <blockdevice.h>
#include <encoding.h>
#include <atapio.h>
#include <heap.h>
#include <ata.h>
#include <pci.h>
#include <irq.h>
#include <io.h>

uint64_t sectors_count;
uint32_t bytes_per_sector;
bool is_lba48;

bool irq_recieved;
bool is_secondary;
bool is_slave;

int atapio_sendcmd(atacmd_t ata_cmd, bool panic);
void atapio_selectsector(uint64_t lba, uint32_t count);
void atapio_dummy();
void atapio_handler();

void atapio_init(bool secondary, bool slave)
{
    is_secondary = secondary;
    is_slave = slave;

    blockdevice_t *ata_block;
    memcpy(ata_block->signature, "BLKDEV", 6);
    memcpy(ata_block->dev_signature, "SATA", 4);
    ata_block->is_controller = 1;
    ata_block->is_packet = 0;
    
    ata_block->read_sector = atapio_read;
    ata_block->write_sector = atapio_write;

    atapio_sendcmd(ATA_CMD_IDENTIFY, true);
    uint16_t *ata_identifydata = memalloc(512);
    insw(IO_ATA_DATA, ata_identifydata, 256);

    change_endianness(ata_identifydata + 27, 40);
    change_endianness(ata_identifydata + 10, 20);
    change_endianness(ata_identifydata + 23, 8);
    
    memcpy(ata_block->model_name, ata_identifydata + 27, 40);
    memcpy(ata_block->serial_no, ata_identifydata + 10, 20);
    memcpy(ata_block->firmware_rev, ata_identifydata + 23, 8);

    if (is_lba48) {
        sectors_count = *(uint64_t *)(&ata_identifydata[100]);
    } else {
        sectors_count = *(uint32_t *)(&ata_identifydata[60]);
    }
    bytes_per_sector = 512;

    // ATA Needed things
    bool *ata_specifics = memalloc(1);
    ata_specifics[0] = is_secondary;
    ata_specifics[1] = is_slave;
    ata_block->dev_specifics = ata_specifics;

    add_blockdevice(ata_block);
}

int atapio_sendcmd(atacmd_t ata_cmd, bool panic)
{
    outb((is_secondary ? 0x376 : 0x3F6), inb((is_secondary ? 0x376 : 0x3F6)) | (1 << 0));

    outb(IO_ATA_COMMAND, ata_cmd);

    int status;

    do status = inb(IO_ATA_STATUS);
    while ((status & 0x80) && !(status & 0x08));

    if (panic && status & 0x01)
    {
        printerr("atapio: error occured while sending command!", inb(IO_ATA_FEATURES), true); 
        for (;;) ;
    }

    return status;
}

void atapio_selectsector(uint64_t lba, uint32_t count)
{
    if (is_lba48) {
        outb(IO_ATA_DRIVE, 0x40 | is_slave << 4);
        outb(IO_ATA_FEATURES, 0);
        outb(IO_ATA_SECTORS, count >> 8);
        outb(IO_ATA_LBA0, lba >> 24);
        outb(IO_ATA_LBA1, lba >> 32);
        outb(IO_ATA_LBA2, lba >> 40);
        outb(IO_ATA_SECTORS, count);
        outb(IO_ATA_LBA0, lba >> 0);
        outb(IO_ATA_LBA1, lba >> 8);
        outb(IO_ATA_LBA2, lba >> 16);
    } else {
        outb(IO_ATA_DRIVE, 0xE0 | is_slave << 4 | ((lba >> 24) & 0x0F));
        outb(IO_ATA_FEATURES, 0);
        outb(IO_ATA_SECTORS, count);
        outb(IO_ATA_LBA0, lba >> 0);
        outb(IO_ATA_LBA1, lba >> 8);
        outb(IO_ATA_LBA2, lba >> 16);
    }
}

void atapio_dummy()
{
    //inb(IO_ATA_STATUS); 
    //inb(IO_ATA_STATUS);
    //inb(IO_ATA_STATUS); 
    //inb(IO_ATA_STATUS);
    //while(!irq_recieved);
    //_irq_remove_handler(0xE | is_secondary);
    //inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    //inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    //inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    //inb(IO_ATA_STATUS); inb(IO_ATA_STATUS); 
    //inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    //inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    //inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    //inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    //inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
}

void atapio_handler() { irq_recieved = true; }

bool atapio_read(blockdevice_t *ata_block, uint32_t lba, uint32_t count, uint8_t* buffer)
{
    //if (count > ata_block.sectors_count) {
    //    return false;
    //} else if (lba > ata_block.sectors_count) {
    //    return false;
    //}

    // Restore ATA Needed things
    bool *ata_specifics = ata_block->dev_specifics;
    is_secondary = ata_specifics[0];
    is_slave = ata_specifics[1];

    // Restore Correct IO Ports
    ata_getports(is_secondary);

    //printchr('\n');
    //printnum(is_slave);

    atapio_selectsector(lba, count);
    int status = atapio_sendcmd(ATA_CMD_READ_PIO, true);
    
    uint16_t *buffer_16bit = buffer;
    for (uint32_t i = 0; i < 256 * count; i++) {
        while (inb(IO_ATA_STATUS) & 0x80);
        buffer_16bit[i] = inw(IO_ATA_DATA);
    }

    return true;
}

bool atapio_write(blockdevice_t *ata_block, uint32_t lba, uint32_t count, uint8_t* buffer)
{    
    //if (count > sectors_count) {
    //    return false;
    //} else if (lba > sectors_count) {
    //    return false;
    //}
    
    // Restore ATA Needed things
    bool *ata_specifics = ata_block->dev_specifics;
    is_secondary = ata_specifics[0];
    is_slave = ata_specifics[1];

    atapio_selectsector(lba, count);
    int status = atapio_sendcmd((is_lba48 ? ATA_CMD_WRITE_PIO_EXT : ATA_CMD_WRITE_PIO), true);
    //outsw(IO_ATA_DATA, buffer, bytes_per_sector * count);
    for (int i = 0; i < sizeof(buffer) / sizeof(uint16_t); i++)
    {
        outw(IO_ATA_DATA, buffer[i*2+1] << 8 | buffer[i*2]);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    }

    atapio_sendcmd((is_lba48 ? ATA_CMD_FLUSH_CACHE_EXT : ATA_CMD_FLUSH_CACHE), true);

    return status;
}