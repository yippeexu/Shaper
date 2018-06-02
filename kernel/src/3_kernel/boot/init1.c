
#include <sysprint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <system.h>

#include <ps2_keyboard.h>
#include <blockdevice.h>
#include <shapervfs.h>
#include <ps2_mouse.h>
#include <serial.h>
#include <heap.h>
#include <vesa.h>
#include <mbr.h>
#include <vga.h>
#include <irq.h>
#include <rtc.h>
#include <pit.h>
#include <io.h>

void init1()
{
    printinf("hal: looking for pci devices");
    pci_init();

    printinf("hal: initializing system timer");
    pit_init();

    printinf("hal: initializing ps2 keyboard");
    keyboard_init();
    
    // printinf("hal: initializing rtc chip");
    // rtc_init();

    printinf("hal: gathering partitions");
    for (int i = 0; i < 256; i++)
    {
        if (!memcmp(blockdevices[i]->signature, "BLKDEV", 6)) break;
        
        if (blockdevices[i]->is_controller == 0) continue;
        mbr_getparts(blockdevices[i]);
    }
    
    printinf("hal: initializing partitions");
    for (int i = 0; i < 256; i++) 
    {
        if (!memcmp(blockdevices[i]->signature, "BLKDEV", 6)) break;

        if (!blockdevices[i]->is_controller && !blockdevices[i]->is_packet)
           vfs_init(blockdevices[i]);
    }
}