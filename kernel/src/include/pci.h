
#include <stdbool.h>
#include <stdint.h>

#ifndef $_PCI_H
#define $_PCI_H

typedef struct tag_pci_device_bar
{ 
    uint32_t base_address;
    uint32_t base_address_upper;
    bool is_io;
    bool is_prefetchable;
} pcidevice_bar_t;

typedef struct tag_pci_device
{
    uint8_t bus;
    uint8_t slot;
    uint8_t func;

    uint16_t vendor_id;
    uint16_t device_id;
    
    uint16_t command;
    uint16_t status;

    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass_code;
    uint8_t class_code;

    uint8_t cacheline_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;

    pcidevice_bar_t bar[6];

    uint32_t cardbus_cis;

    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;
    
    uint32_t rom_base_addrses;

    uint8_t capabilities_ptr;
    uint8_t reserved[7];

    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;

} pcidevice_t;

void pci_init();
void pci_enable_io(pcidevice_t *pci_device);
void pci_disable_io(pcidevice_t *pci_device);
void pci_enable_memory(pcidevice_t *pci_device);
void pci_disable_memory(pcidevice_t *pci_device);
void pci_enable_busmaster(pcidevice_t *pci_device);
void pci_disable_busmaster(pcidevice_t *pci_device);
void pci_set_latency(pcidevice_t *pci_device, uint8_t new_timer);

#endif