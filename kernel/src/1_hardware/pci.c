
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <blockdevice.h>
#include <pit.h>
#include <pci.h>
#include <io.h>

/* Block devices */
#include <ahci.h>
#include <ata.h>
/* ------------- */

bool allowed_buses[256];
pcidevice_t *pci_devices;
int device_length;

void pci_scan();
void pci_checkbus(uint8_t bus);
void pci_checkslot(uint8_t bus, uint8_t slot);
void pci_checkfunc(uint8_t bus, uint8_t slot, uint8_t func);
uint8_t pci_read8(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t pci_read16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pci_read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_readall(uint8_t bus, uint8_t slot, uint8_t func, pcidevice_t *buffer);
void pci_write8(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t value);
void pci_write16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t value);
void pci_write32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value);

void pci_init()
{
    pci_scan();
}

void pci_scan()
{
    pci_devices = memalloc(0x40000);

    for (int bus = 0; bus < 256; bus++)
        for (int slot = 0; slot < 32; slot++)
            pci_checkslot(bus, slot);

    for (int i = 0; i < device_length; i++)
    {
        if (pci_devices[i].class_code == 0x01)
        {
            //if (pci_devices[i].subclass_code == 0x01)
            //{
            //    //if (pci_devices[i].prog_if == 0x00)
            //    //{
            //        printstr("Found an ATA IDE Controller!\n");
            //        for (int i = 0; i < 4; i++)
            //        {
            //            printstr("BAR-");
            //            printnum(i);
            //            printstr((pci_devices[i].bar[i].is_io ? " I/O" : " Memory-mapped I/O"));
            //            printstr(" Base Address: ");
            //            printhex(pci_devices[i].bar[i].base_address, 0);
            //            printchr('\n');
            //        }
            //        //ata_init(&pci_devices[i], false, false);
            //        //ata_init(&pci_devices[i], false, true);
            //        //ata_init(&pci_devices[i], true, false);
            //        //ata_init(&pci_devices[i], true, true);
            //    //}
            //}
            /*else */if (pci_devices[i].subclass_code == 0x06)
            {
               if (pci_devices[i].prog_if == 0x01)
               {
                   ahci_init(&pci_devices[i]);
               }
            }
        }
    }
}

void pci_checkbus(uint8_t bus)
{
    for (uint8_t slot = 0; slot < 32; slot++)
        pci_checkslot(bus, slot);
}

void pci_checkslot(uint8_t bus, uint8_t slot)
{
    if (pci_read16(bus, slot, 0, 0x00) == 0xFFFF) return;
    
    if (pci_read8(bus, slot, 0, 0x0E) & 0x80) {
        for (uint8_t func = 1; func < 8; func++)
        {
            if (pci_read16(bus, slot, func, 0x00) != 0xFFFF) {
                pci_checkfunc(bus, slot, func);
            }
        }
    } else pci_checkfunc(bus, slot, 0);
}

void pci_checkfunc(uint8_t bus, uint8_t slot, uint8_t func)
{
    pcidevice_t new_device;
    pci_readall(bus, slot, func, &new_device);

    printstr("pci: ");
    printstr(" id ");
    printstr(hex2str(new_device.class_code, 2));
    printstr(hex2str(new_device.subclass_code, 2));
    printstr(" | vendor ");
    printstr(hex2str(new_device.vendor_id, 4));
    printstr(" device ");
    printstr(hex2str(new_device.device_id, 4));
    printchr('\n');

    //sleep(500);

    if (new_device.class_code == 0x06 && new_device.subclass_code == 0x04)
    {
        pci_checkbus(pci_read8(bus, slot, func, 0x19));
        //allowed_buses[pci_read8(bus, slot, func, 0x15)] = false;
    }

    pci_devices[device_length++] = new_device;
}

void pci_readall(uint8_t bus, uint8_t slot, uint8_t func, pcidevice_t *buffer)
{
    if (pci_read8(bus, slot, func, 0x0E) == 0x00)
    {
        uint32_t bar, bar2;
        for (int i = 0; i < 6; i++)
        {
            bar = pci_read32(bus, slot, func, 0x10+0x04*i);
            bar2 = pci_read32(bus, slot, func, 0x14+0x04*i);
            buffer->bar[i].is_io = bar & 1;
            if (bar & 1) {
                buffer->bar[i].is_prefetchable = 0;
                buffer->bar[i].base_address = bar & 0xFFFFFFFC;
            } else {
                buffer->bar[i].is_prefetchable = (bar >> 3) & 1;
                if (((bar >> 1) & 3) == 0x00)
                    buffer->bar[i].base_address = bar & 0xFFFFFFF0;
                else if (((bar >> 1) & 3) == 0x02) {
                    buffer->bar[i].base_address_upper = bar2 & 0xFFFFFFFF; 
                    buffer->bar[i].base_address = bar & 0xFFFFFFF0;
                } else if (((bar >> 1) & 3) == 0x01)
                    buffer->bar[i].base_address = bar & 0xFFF0;
            }
        }
        uint8_t subclass = pci_read8(bus, slot, func, 0x0A), class = pci_read8(bus, slot, func, 0x0B);
        buffer->bus = bus;
        buffer->slot = slot;
        buffer->func = func;
        buffer->vendor_id = pci_read16(bus, slot, func, 0x00);
        buffer->device_id = pci_read16(bus, slot, func, 0x02);
        buffer->command = pci_read16(bus, slot, func, 0x04);
        buffer->status = pci_read16(bus, slot, func, 0x06);
        buffer->revision_id = pci_read8(bus, slot, func, 0x08);
        buffer->prog_if = pci_read8(bus, slot, func, 0x09);
        buffer->subclass_code = subclass;
        buffer->class_code = class;
        buffer->cacheline_size = pci_read8(bus, slot, func, 0x0C);
        buffer->latency_timer = pci_read8(bus, slot, func, 0x0D);
        buffer->header_type = pci_read8(bus, slot, func, 0x0E);
        buffer->bist = pci_read8(bus, slot, func, 0x0F);
        buffer->cardbus_cis = pci_read32(bus, slot, func, 0x28);
        buffer->subsystem_vendor_id = pci_read16(bus, slot, func, 0x2C);
        buffer->subsystem_id = pci_read16(bus, slot, func, 0x2E);
        buffer->rom_base_addrses = pci_read32(bus, slot, func, 0x30);
        buffer->capabilities_ptr = pci_read8(bus, slot, func, 0x34);
        buffer->interrupt_line = pci_read8(bus, slot, func, 0x3C);
        buffer->interrupt_pin = pci_read8(bus, slot, func, 0x3D);
        buffer->min_grant = pci_read8(bus, slot, func, 0x3E);
        buffer->max_latency = pci_read8(bus, slot, func, 0x3F);
    }
}

uint8_t pci_read8(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t base_address = (0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC));
    outl(0xCF8, base_address);
    return ((inl(0xCFC) >> ((offset % 4) * 8)) & 0xFF);
}

uint16_t pci_read16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t base_address = (0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC));
    outl(0xCF8, base_address);
    return ((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
}

uint32_t pci_read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t base_address = (0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC));
    outl(0xCF8, base_address);
    return ((inl(0xCFC) >> ((offset & 3) * 8)));
}

void pci_write8(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t value)
{
    uint32_t base_address = (0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC));
    outl(0xCF8, base_address);
    outb(0xCFC, value);
}

void pci_write16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t value)
{
    uint32_t base_address = (0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC));
    outl(0xCF8, base_address);
    outw(0xCFC, value);
}

void pci_write32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value)
{
    uint32_t base_address = (0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC));
    outl(0xCF8, base_address);
    outl(0xCFC, value);
}

void pci_enable_io(pcidevice_t *pci_device)
{
    uint16_t command = pci_read16(pci_device->bus, pci_device->slot, pci_device->func, 0x04);
    command |= 1 << 0;
    pci_write16(pci_device->bus, pci_device->slot, pci_device->func, 0x04, command);
}

void pci_enable_memory(pcidevice_t *pci_device)
{
    uint16_t command = pci_read16(pci_device->bus, pci_device->slot, pci_device->func, 0x04);
    command |= 1 << 1;
    pci_write16(pci_device->bus, pci_device->slot, pci_device->func, 0x04, command);
}

void pci_enable_busmaster(pcidevice_t *pci_device)
{
    uint16_t command = pci_read16(pci_device->bus, pci_device->slot, pci_device->func, 0x04);
    command |= 1 << 2;
    pci_write16(pci_device->bus, pci_device->slot, pci_device->func, 0x04, command);
}

void pci_disable_io(pcidevice_t *pci_device)
{
    uint16_t command = pci_read16(pci_device->bus, pci_device->slot, pci_device->func, 0x04);
    command &= ~(1 << 0);
    pci_write16(pci_device->bus, pci_device->slot, pci_device->func, 0x04, command);
}

void pci_disable_memory(pcidevice_t *pci_device)
{
    uint16_t command = pci_read16(pci_device->bus, pci_device->slot, pci_device->func, 0x04);
    command &= ~(1 << 1);
    pci_write16(pci_device->bus, pci_device->slot, pci_device->func, 0x04, command);
}

void pci_disable_busmaster(pcidevice_t *pci_device)
{
    uint16_t command = pci_read16(pci_device->bus, pci_device->slot, pci_device->func, 0x04);
    command &= ~(1 << 2);
    pci_write16(pci_device->bus, pci_device->slot, pci_device->func, 0x04, command);
}

void pci_set_latency(pcidevice_t *pci_device, uint8_t new_timer)
{
    pci_write8(pci_device->bus, pci_device->slot, pci_device->func, 0x0D, new_timer);
}