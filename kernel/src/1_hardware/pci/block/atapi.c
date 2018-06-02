
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <blockdevice.h>
#include <atapi.h>
#include <ata.h>
#include <io.h>

uint64_t sectors_count;
uint32_t bytes_per_sector;
char *model_name;
char *firmware_rev;
char *serial_no;
bool irq_recieved;
bool is_secondary;
bool is_slave;

uint16_t* atapi_buffer;

int atapi_sendpacketcmd(atacmd_t ata_cmd, atapi_packet_t ata_packet, bool panic);
atapi_packet_t atapi_setpacket(uint64_t lba, uint64_t count);
int atapi_sendcmd(atacmd_t ata_cmd, bool panic);

void atapi_init(bool secondary, bool slave)
{
    bytes_per_sector = 2048;
    is_secondary = secondary;
    is_slave = slave;

    atapi_buffer = memalloc(0x200*0x20);

    blockdevice_t *atapi_block;
    atapi_block->is_controller = 1;
    atapi_block->is_packet = 1;

    atapi_block->read_sector = atapi_read;
    atapi_block->write_sector = atapi_write;

    //atapi_sendcmd(ATA_CMD_IDENTIFY_PACKET, true);
    //uint16_t *atapi_identifydata = insw(IO_ATA_DATA, 256);
    
    //model_name = atapi_identifydata + 27;
    //serial_no = atapi_identifydata + 10;
    //firmware_rev = atapi_identifydata + 23;
    //change_endianness(model_name, 40);
    //change_endianness(serial_no, 20);
    //change_endianness(firmware_rev, 8);
    
    //atapi_block.model_name = model_name;
    //atapi_block.serial_no = serial_no;
    //atapi_block.firmware_rev = firmware_rev;

    bool *ata_specifics = memalloc(1);
    ata_specifics[0] = is_secondary;
    ata_specifics[1] = is_slave;
    atapi_block->dev_specifics = ata_specifics;

    // uint8_t *mybuffer = memalloc(512);
    // atapi_read(0, 1, mybuffer);
    // printchr('\n');
    // for (int i = 0; i < 512; i++) {
    //     if (i != 0 && !(i % 28)) printchr('\n');
    //     printstr(hex2str(mybuffer[i], 2));
    //     printchr(' ');
    // }

    add_blockdevice(atapi_block);
}

int atapi_sendpacketcmd(atacmd_t ata_cmd, atapi_packet_t atapi_packet, bool panic)
{
    atapi_packet.ata_command = ata_cmd;
    outsw(IO_ATA_DATA, (uint16_t *)&atapi_packet, 6);
    inb(IO_ATA_STATUS);
    inb(IO_ATA_STATUS);
    int size = (inb(IO_ATA_LBA2) << 8 | inb(IO_ATA_LBA1));
    //if (size != 2048)
    insw(IO_ATA_DATA, atapi_buffer, size);
    while ((inb(IO_ATA_COMMAND)) & 0x88) __asm volatile ("pause");
    //    return;

    return size;
}

atapi_packet_t atapi_setpacket(uint64_t lba, uint64_t count)
{
    atapi_packet_t atapi_packet;
    int status;
    
    outb(IO_ATA_DRIVE, is_slave & (1 << 4));
    inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    outb(IO_ATA_FEATURES, 0);
    outb(IO_ATA_LBA1, bytes_per_sector & 0xFF);
    outb(IO_ATA_LBA2, bytes_per_sector >> 8);

    outb(IO_ATA_COMMAND, ATA_CMD_PACKET);
    while ((status = inb(IO_ATA_COMMAND)) & 0b10000000) __asm volatile ("pause");
    while (!(status = inb(IO_ATA_COMMAND) & 0b00001000)) __asm volatile ("pause");
    if (status & 0x01)
    {
        printerr("(ATAPI) Error occured while sending packet command!", ERRCODE_NONE, true);
        return;
    }
    atapi_packet.control = count;
    atapi_packet.lba0 = (lba >> 0x00) & 0xFF;
    atapi_packet.lba1 = (lba >> 0x08) & 0xFF;
    atapi_packet.lba2 = (lba >> 0x10) & 0xFF;
    atapi_packet.lba3 = (lba >> 0x18) & 0xFF;
    return atapi_packet;
}

int atapi_sendcmd(atacmd_t ata_cmd, bool panic)
{
    outb(IO_ATA_COMMAND, (uint8_t)ata_cmd);
    if (panic && inb(IO_ATA_STATUS) & 0x01) {
        printerr("(ATAPI) Error occurred while sending command", ERRCODE_NONE, true);
        for (;;) ;
    }
    atapi_dummy();
    return inb(IO_ATA_STATUS);
}

void atapi_dummy()
{
    inb(IO_ATA_STATUS);
    inb(IO_ATA_STATUS);
    inb(IO_ATA_STATUS);
    inb(IO_ATA_STATUS);
    while (!irq_recieved);
    irq_recieved = false;
}

void atapi_handler()
{
    irq_recieved = true;
}

int atapi_read(uint64_t lba, uint64_t count, uint8_t* buffer)
{
    irq_recieved = false;

    atapi_packet_t atapi_packet;
    atapi_packet.ata_command = ATA_CMD_READ;
    atapi_packet.lba0 = (lba >> 0) & 0xFF;
    atapi_packet.lba1 = (lba >> 8) & 0xFF;
    atapi_packet.lba2 = (lba >> 16) & 0xFF;
    atapi_packet.lba3 = (lba >> 24) & 0xFF;
    atapi_packet.control = count;

    outb(IO_ATA_DRIVE, is_slave << 4);
    for (int i; i < 4; i++) inb(IO_ATA_STATUS);

    outb(IO_ATA_FEATURES, 0x00); // PIO mode

    outb(IO_ATA_LBA1, 2048 & 0xFF);
    outb(IO_ATA_LBA2, 2048 >> 8);

    outb(IO_ATA_COMMAND, ATA_CMD_PACKET);

    for (int i; i < 4; i++) inb(IO_ATA_ALTSTATUS);

    int status;
    while (status = inb(IO_ATA_STATUS) & 0x80) ;
    //READ(12) with transfer length <= 0, ok (0)
    if (status & 0x01) {
        printerr("atapi: error occurred while sending packet command", ERRCODE_NONE, true);
        for (;;) ;
    }

    outsw(IO_ATA_DATA, &atapi_packet, 6);

    _irq_add_handler(0xE | is_secondary, atapi_handler);
    for (int i = 0; i < count; i++) {
        printchr('e');
        for (int i = 0; i < 10; i++) inb(IO_ATA_ALTSTATUS);
        //atapi_dummy();
        printchr('e');
        
        int status;
        while (status = inb(IO_ATA_STATUS) & 0x80);
        while (!(status = inb(IO_ATA_STATUS) & 0x08));

        if (status & 0x01) {
            printerr("atapi: error occurred while reading sectors", ERRCODE_NONE, true);
            for (;;) ;
        }

        insw(IO_ATA_DATA, buffer, 1024);
        buffer += 2048;
    }
    for (int i = 0; i < 10; i++) inb(IO_ATA_ALTSTATUS);
    //while (!irq_recieved)
    //irq_recieved = false;
    
    while ((inb(IO_ATA_STATUS) & 0x80));
    //while (inb(IO_ATA_STATUS) & 0x08);
    //while (inb(IO_ATA_ALTSTATUS) & 0x88) ;

    //atapi_sendpacketcmd(ATA_CMD_READ, atapi_setpacket(lba, count), true);    
    //buffer = atapi_buffer;
    //buffer = insw(IO_ATA_DATA, bytes_per_sector * count);
}

int atapi_write(uint64_t lba, uint64_t count, uint8_t* buffer)
{
    atapi_sendpacketcmd(ATA_CMD_WRITE, atapi_setpacket(lba, count), true);    
    for (int i = 0; i < sizeof(buffer) / sizeof(uint16_t); i++)
    {
        outb(IO_ATA_DATA, buffer[i*2+1] << 8 | buffer[i*2]);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
        inb(IO_ATA_STATUS); inb(IO_ATA_STATUS);
    }

    while (inb(IO_ATA_COMMAND) & 0x88) __asm volatile ("pause");
    atapi_sendpacketcmd(ATA_CMD_FLUSH_CACHE, atapi_setpacket(0, 0), true);    
    while (inb(IO_ATA_COMMAND) & 0x88) __asm volatile ("pause");
}
