
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <blockdevice.h>
#include <ahci.h>
#include <sata.h>
#include <pci.h>

#define IPM_NOT_PRESENT 0x00
#define IPM_ACTIVE_STATE 0x01
#define IPM_PARTIAL_STATE 0x02
#define IPM_SLUMBER_STATE 0x06
#define IPM_DEVSLEEP_STATE 0x08

#define DET_NOT_DETECTED 0x00
#define DET_DETECTED_NOT_ESTABLISHED 0x01
#define DET_DETECTED_ESTABLISHED 0x03
#define DET_OFFLINE 0x04

ahci_generic_t *ahci_mem;
ahci_port_t *ahci_ports;
ahci_drivetype_t ahci_port_type[32];

char *capabilities_string[] =
{
    0,
    0,
    0,
    0,
    0,
    "supports external sata",
    "enclosure management supported",
    "command completion coalescing supported",
    0,
    0,
    0,
    0,
    0,
    "partial state capable",
    "slumber state capable",
    "pio multiple drq block",
    "fis-based switching supported",
    "supports port multiplier",
    "supports ahci mode only",
    0,    
    0,    
    0,    
    0,    
    "supports command list override",
    "supports activity led",
    "supports aggressive link power management",
    "supports staggered spin-up",    
    "supports mechanical presence switch",
    "supports sntf register",
    "supports native command queuing",
    "supports 64-bit addressing"
};

char *interface_speeds[] =
{
    0,
    "1.5 Gbps",
    "3 Gbps",
    "6 Gbps",
};

ahci_drivetype_t ahci_getdrivetype(uint32_t port_signature, int port_number);
void ahci_read_capabilisties();
void ahci_scanports();

void ahci_init(pcidevice_t *ahci_device)
{
    //pci_enable_busmaster(ahci_device, true);
    //pci_enable_memory(ahci_device, true);

    ahci_mem = (ahci_generic_t *)(ahci_device->bar[5].base_address);
    ahci_ports = (ahci_port_t *)((ahci_device->bar[5].base_address) + 0x100);

    ahci_read_capabilities();

    printstrf("%c1%n");
    printstr("ahci: found ahci controllers with ");
    printnum(ports_count);
    if (ports_count > 1) printstr(" ports.\n");
    else printstr(" port.\n");
    
    ahci_scanports();

    int spin = 0;
    ahci_mem->global_control |= (1 << 31);
    while (!((ahci_mem->global_control >> 31) & 1) && spin < 100000)
    {
        spin++;
    }
    if (spin == 100000)
    {
        printerr("ahci: controller not responding!", ERRCODE_NONE, true);
    }
    printstr("ahci: ahci enabled\n");

    for (int i = 0; i < ports_count; i++)
    {
        if (ahci_port_type[i] == AHCI_DRIVE_SATA)
        {
            sata_init(ahci_mem, &ahci_ports[i], i);
        }
    }
}

void ahci_read_capabilities()
{
    ports_count = (ahci_mem->capabilities >> 0) & 0x1F;
    supports_externalsata = (ahci_mem->capabilities >> 5) & 1;
    em_supported = (ahci_mem->capabilities >> 6) & 1;
    ccc_supported = (ahci_mem->capabilities >> 7) & 1;
    cmd_slots = (ahci_mem->capabilities >> 8) & 0x1F;
    partial_state_capable = (ahci_mem->capabilities >> 13) & 1;
    slumber_state_capable = (ahci_mem->capabilities >> 14) & 1;
    pio_multiple_drq_block = (ahci_mem->capabilities >> 15) & 1;
    fb_switching_supported = (ahci_mem->capabilities >> 16) & 1;
    supports_portmultiplier = (ahci_mem->capabilities >> 17) & 1;
    supports_ahcionly = (ahci_mem->capabilities >> 18) & 1;
    interface_speed_support = (ahci_mem->capabilities >> 20) & 0x0F;
    supports_cmdlist_override = (ahci_mem->capabilities >> 24) & 1;
    supports_activity_led = (ahci_mem->capabilities >> 25) & 1;
    supports_aggressive_link_power_management = (ahci_mem->capabilities >> 26) & 1;
    supports_staggered_spinup = (ahci_mem->capabilities >> 27) & 1;
    supports_mechanical_presence_switch = (ahci_mem->capabilities >> 28) & 1;
    supports_snotification_register = (ahci_mem->capabilities >> 29) & 1;
    supports_native_command_queuing = (ahci_mem->capabilities >> 30) & 1;
    supports_64bit_addressing = (ahci_mem->capabilities >> 31) & 1;

    //for (int i = 0; i < 32; i++)
    //{
    //    if ((ahci_mem->capabilities >> i) & 1)
    //    {
    //        if (capabilities_string[i])
    //        {
    //            printstr("ahci: capabilities: ");
    //            printstr(capabilities_string[i]);
    //            printchr('\n');
    //        }
    //    }
    //}
} 

void ahci_scanports()
{
    uint32_t ports = ahci_mem->ports_implemented;
    for (int i = 0; i < ports_count; i++)
    {
        if (ports & 1)
        {
            ahci_port_type[i] = ahci_getdrivetype(ahci_ports[i].port_signature, i);
            switch (ahci_port_type[i])
            {
            case AHCI_DRIVE_SATA:
                printstr("ahci: found sata port in ");
                printnum(0);
                printstr(":");
                printnum(i);
                printchr('\n');
                break;
            case AHCI_DRIVE_SATAPI:
                printstr("ahci: found satapi port in ");
                printnum(0);
                printstr(":");
                printnum(i);
                printchr('\n');
                break;
            case AHCI_DRIVE_PORT_MULTIPLIER:
                printstr("ahci: found port multiplier in ");
                printnum(0);
                printstr(":");
                printnum(i);
                printchr('\n');
                break;
            case AHCI_DRIVE_SEMB:
                printstr("ahci: found serial enclosure management bridge in ");
                printnum(0);
                printstr(":");
                printnum(i);
                printchr('\n');
                break;
            case AHCI_DRIVE_NULL:
                printstr("ahci: found null port in ");
                printnum(0);
                printstr(":");
                printnum(i);
                printchr('\n');
                break;
            }
        }
    }
}

ahci_drivetype_t ahci_getdrivetype(uint32_t port_signature, int port_number)
{
    uint32_t interface_power_management;
    uint32_t device_detection;

    interface_power_management = (ahci_ports[port_number].sata_status >> 8) & 0x0F;
    device_detection = (ahci_ports[port_number].sata_status & 0x0F);

    if (interface_power_management != IPM_ACTIVE_STATE)
        return AHCI_DRIVE_NOTHING;
    if (device_detection != DET_DETECTED_ESTABLISHED)
        return AHCI_DRIVE_NOTHING;

    switch (port_signature >> 16)
    {
    case 0xC33C: return AHCI_DRIVE_SEMB;
    case 0xEB14: return AHCI_DRIVE_SATAPI;
    case 0x9669: return AHCI_DRIVE_PORT_MULTIPLIER;
    default:     return AHCI_DRIVE_SATA;
    }
}