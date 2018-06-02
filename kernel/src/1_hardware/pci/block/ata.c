
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <atapio.h>
#include <atapi.h>
#include <pci.h>
#include <irq.h>
#include <ata.h>
#include <io.h>

bool is_slave;
bool is_secondary;
bool irq_recieved;

int ata_sendcmd(atacmd_t ata_cmd, bool panic);
ata_drive_t ata_scandrives();
void ata_dummy();

void ata_init(pcidevice_t *ata_device, bool secondary, bool slave)
{
    //if (ata_device) {
        is_slave = slave;
        is_secondary = secondary;

        // Get Correct IO Ports
        ata_getports(is_secondary);

        ata_drive_t drivetype;
        drivetype = ata_scandrives();
        if (!is_secondary && is_slave) return;
        if (drivetype == ATA_DRIVE_ATA) {
            // ATA Pio Mode
            atapio_init(is_secondary, is_slave);
        } else if (drivetype == ATA_DRIVE_ATAPI) {
            // ATAPI
            return;
            //atapi_init(is_secondary, is_slave);
        } else if (drivetype == ATA_DRIVE_NOTHING) {
            printstr("ide: nothing on this device!\n");
        }
    //}
}

ata_drive_t ata_scandrives(void)
{
    printstr("ide: scanning for a drive on ata ");
    printnum(is_secondary);
    printchr(':');
    printnum(is_slave);
    printchr('\n');

    outb(IO_ATA_DRIVE, 0xA0 | (is_slave << 4));
    io_sleep(); io_sleep(); io_sleep(); io_sleep();

    outb(IO_ATA_COMMAND, ATA_CMD_IDENTIFY);
    io_sleep(); io_sleep(); io_sleep(); io_sleep();

    if (inb(IO_ATA_STATUS) == 0) {
        printstr("ide: no drive found in ata ");
        printnum(is_secondary);
        printchr(':');
        printnum(is_slave);
        printchr('\n');
        return ATA_DRIVE_NOTHING; // Drive doesn't exists
    }

    int status;
    while (true) 
    {
        printnum(0);
        status = inb(IO_ATA_STATUS);
        if (status & 0x01) break;
        if ((status & 0x08) && !(status & 0x80)) break;
    }

    if (status & 0x01)
    {
        uint16_t signature = inb(IO_ATA_LBA2) << 8 | inb(IO_ATA_LBA1);

        if (signature == 0x9669) {
            printstr("ide: found atapi drive in ata ");
            printnum(is_secondary);
            printchr(':');
            printnum(is_slave);
            printchr('\n');
            return ATA_DRIVE_ATAPI;
        } else if (signature == 0xEB14) {
            printstr("ide: found atapi drive in ata ");
            printnum(is_secondary);
            printchr(':');
            printnum(is_slave);
            printchr('\n');
            return ATA_DRIVE_ATAPI;
        } else {
            printstr("ide: found null drive in ata ");
            printnum(is_secondary);
            printchr(':');
            printnum(is_slave);
            printchr('\n');
            return ATA_DRIVE_NULL;
        }
    }

    printstr("ide: found ata drive in ata ");
    printnum(is_secondary);
    printchr(':');
    printnum(is_slave);
    printchr('\n');

    return ATA_DRIVE_ATA;
}

int ata_sendcmd(atacmd_t ata_cmd, bool panic)
{
    //_irq_add_handler(0xE | is_secondary, ata_handler);
    //outb(IO_ATA_COMMAND, (uint8_t)ata_cmd);
    //if (panic && inb(IO_ATA_STATUS) & 0x01) {
    //    printerr("(ATA) Error occurred while sending command", ERRCODE_NONE, true);
    //    for (;;) ;
    //}
    //ata_dummy();
    //return inb(IO_ATA_STATUS);
}

//void ata_dummy()
//{
//    inb(IO_ATA_STATUS);
//    inb(IO_ATA_STATUS);
//    inb(IO_ATA_STATUS);
//    inb(IO_ATA_STATUS);
//    while (!irq_recieved);
//    _irq_remove_handler(0xE | is_secondary);
//}
//
//void ata_handler() { irq_recieved = true; }

void ata_getports(bool secondary)
{
    IO_ATA_DATA = (secondary ? IO_ATA_SECONDARY_DATA : IO_ATA_PRIMARY_DATA);
    IO_ATA_FEATURES = (secondary ? IO_ATA_SECONDARY_FEATURES : IO_ATA_PRIMARY_FEATURES);
    IO_ATA_SECTORS = (secondary ? IO_ATA_SECONDARY_SECTORS : IO_ATA_PRIMARY_SECTORS);
    IO_ATA_LBA0 = (secondary ? IO_ATA_SECONDARY_LBA0 : IO_ATA_PRIMARY_LBA0);
    IO_ATA_LBA1 = (secondary ? IO_ATA_SECONDARY_LBA1 : IO_ATA_PRIMARY_LBA1);
    IO_ATA_LBA2 = (secondary ? IO_ATA_SECONDARY_LBA2 : IO_ATA_PRIMARY_LBA2);
    IO_ATA_DRIVE = (secondary ? IO_ATA_SECONDARY_DRIVE : IO_ATA_PRIMARY_DRIVE);
    IO_ATA_COMMAND = (secondary ? IO_ATA_SECONDARY_COMMAND : IO_ATA_PRIMARY_COMMAND);
    IO_ATA_STATUS = (secondary ? IO_ATA_SECONDARY_STATUS : IO_ATA_PRIMARY_STATUS);
    IO_ATA_ALTSTATUS = (secondary ? IO_ATA_SECONDARY_ALTSTATUS : IO_ATA_PRIMARY_ALTSTATUS);
};
